<?php
// $Name$

/**
 * @file
 * upload functions
 */

/**
 * Menu callback; presents the upload form for imagepicker
 */
// iframe
function imagepicker_upload() {
  variable_del('imagepicker_advanced_browser_pagestart');
  $content .= "<div class='imgp_help'>" . t('Upload images. You can give them a title and description') ."</div>";
  $content = drupal_get_form('imagepicker_upload_form');
  theme('imagepicker', $content);
}
// account
function imagepicker_user_upload() {
  variable_del('imagepicker_advanced_browser_pagestart');
  $content .= "<div class='imgp_help'>" . t('Upload images. You can give them a title and description') ."</div>";
  $content .= drupal_get_form('imagepicker_upload_form', TRUE);
  return $content;
}

function imagepicker_upload_form(&$form_state, $account=FALSE) {
  $form['#attributes']['enctype'] = 'multipart/form-data';
  $form['file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Image file'),
    '#description' => t('Browse your computer for image file'),
    '#required' => TRUE,
    '#value' => 1
  );
  $form['thumb'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail size'),
    '#size' => 10,
    '#default_value' => variable_get('imagepicker_default_thumbnail_size', 100),
    '#description' => t('Size in pixels of thumbnail\'s bigger side'),
    '#required' => TRUE
  );
  $form['scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Scale image'),
    '#size' => 10,
    '#default_value' => variable_get('imagepicker_default_scale', ''),
    '#description' => t('Scale image to this size in pixels if not left empty')
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Add a title for your image')
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#rows' => 2,
    '#cols' => 80,
    '#description' => t('Add a description for your image')
  );
  $form['account'] = array(
    '#type' => 'hidden',
    '#value' => $account,
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  return $form;
}

/**
 * Validate form
 */
function imagepicker_upload_form_validate($form, &$form_state) {
  foreach ($form_state['values'] as $name => $value) {
    $value = trim($value);
    switch ($name) {
      case 'file_upload':
        if (empty($_FILES['files']['name']['file_upload'])) {
          form_set_error($name, t('Image file field is required.'));
        }
        elseif (!isset($_FILES['files']['tmp_name']['file_upload']) || !file_exists($_FILES['files']['tmp_name']['file_upload'])) {
          form_set_error($name, t('Error while uploading file.'));
        }
        elseif (!image_get_info($_FILES['files']['tmp_name']['file_upload'])) {
          form_set_error($name, t('Uploaded file is not an image.'));
        }
        elseif (!imagepicker_get_uploaded_file_extension('file_upload')) {
          form_set_error($name, t('Only .jpg, .gif and .png image files are accepted.'));
        }
        break;

      case 'thumb':
        if (!preg_match('/^[0-9]{1,3}$/', $value) || $value <= 0) {
          form_set_error($name, t('Thumbnail size should be an integer between 1 and 999.'));
        }
        break;

      case 'scale':
        if (!preg_match('/^[0-9]{0,3}$/', $value)) {
          form_set_error($name, t('Scale value should be an integer between 1 and 999 or leave it empty if you don\'t want to scale your image.'));
        }
        break;
    }
  }
}

/**
 * Submit form
 */
function imagepicker_upload_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Upload')) {
    global $user;
    $destination = imagepicker_get_path(FALSE, TRUE);
    $thumbsdir = $destination .'thumbs';
    $browserdir = $destination .'browser';

    if (file_check_directory($destination, TRUE) && file_check_directory($thumbsdir, TRUE) && file_check_directory($browserdir, TRUE)) {
      // Add DIRECTORY_SEPARATORS here because drupals' functions remove trailing slashes
      $destination .= DIRECTORY_SEPARATOR;
      $thumbsdir = $thumbsdir . DIRECTORY_SEPARATOR;
      $browserdir = $browserdir . DIRECTORY_SEPARATOR;

      $maxthumbsize = $form_state['values']['thumb'] ? $form_state['values']['thumb'] : 100;
      $scaleto = $form_state['values']['scale'] ? $form_state['values']['scale'] : FALSE;

      if (!$scaleto) {
        // Use $path instead of original $destination variable cause this
        // variable's value will be changed during copying file, so we won't
        // loose it.
        $path = $destination;
        $imagemoved = imagepicker_copy_uploaded_file($path, 'file_upload');
        $file = basename($path);
      }
      else {
        $source = $_FILES['files']['tmp_name']['file_upload'];
        $file = imagepicker_get_uploaded_file_name($destination, 'file_upload');
        $imagescaled = imagepicker_scale_image($source, $destination . $file, $scaleto);
      }

      if (!$scaleto && $imagemoved || $scaleto && $imagescaled) {

        // Source file should still be an uploaded one, as scaled image
        // might have some watermarks etc. from drupal's filters/hooks.
        $source = $_FILES['files']['tmp_name']['file_upload'];

        if (imagepicker_scale_image($source, $thumbsdir . $file, $maxthumbsize)) {
          imagepicker_scale_image($source, $browserdir . $file, variable_get('imagepicker_default_browser_thumbnail_size', 100));
          $title = htmlspecialchars($form_state['values']['title']);
          $description = htmlspecialchars($form_state['values']['description']);
          $date = date('Y-m-d H:i:s');
          $result = db_query("INSERT INTO {imagepicker} (uid, img_name, img_title, img_description, img_date) VALUES ('%d', '%s', '%s', '%s', '%s')", array($user->uid, $file, $title, $description, $date));
          if ($result) {
            $nextimgid = db_last_insert_id('imagepicker', 'img_id');
            drupal_set_message(t('Image was successfully uploaded.'));
            if ($form_state['values']['account']) {
              $outpath = 'user/'. $user->uid .'/imagepicker/images/browse';
            }
            else {
              $outpath = 'imagepicker/browse/'. $nextimgid;
            }
            drupal_goto($outpath);
          }
          else {
            file_delete($thumbsdir . $file);
            file_delete($browserdir . $file);
            drupal_set_message(t('Error while saving information to database for uploaded image.'), 'error');
          }
        }
        else {
          drupal_set_message(t('Error while creating a thumbnail for uploaded image.'), 'error');
        }
      }
      else {
        if (!$scaleto && !$imagemoved) {
          drupal_set_message(t('Error while moving uploaded file to its destination.'), 'error');
        }
        else {
          drupal_set_message(t('Error while scaling uploaded file.'), 'error');
        }
      }
      file_delete($destination . $file);
    }
    else {
      drupal_set_message(t('Unable to create a directory structure for your images.'), 'error');
    }
  }
  if ($form_state['values']['account']) {
    $outpath = 'user/'. $user->uid .'/imagepicker';
  }
  else {
    $outpath = 'imagepicker';
  }

  drupal_goto($outpath);
}


function imagepicker_copy_uploaded_file(&$destination, $name) {
  $source = $_FILES['files']['tmp_name'][$name];

  if (file_copy($source, $destination, FILE_EXISTS_RENAME)) {
    // Fix bug in drupal's file_copy function which uses '/' instead of
    // DIRECTORY_SEPARATOR for making directories. This causes problems on
    // Windows mashines.
    $source = str_replace('/', DIRECTORY_SEPARATOR, $source);

    $file = imagepicker_get_uploaded_file_name($destination, $name);
    $destination = $destination . $file;
    return @rename($source, $destination);
  }

  return FALSE;
}

function imagepicker_get_uploaded_file_extension($name) {
  switch ($_FILES['files']['type'][$name]) {
    case 'image/pjpeg':
      // "What genius at microsoft decided to rename the mime type for jpgs?"
      // Thats a nice phrase I have found about this mime type :) Wonder what
      // am I talking about? Try to upload some type of jpg image via IE7.
      // Don't know if it's the same with IE6, but IE7 might give you a mime
      // type of image/pjpeg. So lets just treat this 'progressive jpg' as a
      // normal jpg image.
    case 'image/jpeg': $fileext = '.jpg'; break;
    case 'image/gif': $fileext = '.gif'; break;
    case 'image/png': $fileext = '.png'; break;
    default: $fileext = '';
  }

  return $fileext;
}

function imagepicker_get_uploaded_file_name($destination, $name) {
  $fileext = imagepicker_get_uploaded_file_extension($name);

  if (FALSE !== strpos($_FILES['files']['name'][$name], '.')) {
    $filename = drupal_substr($_FILES['files']['name'][$name], 0, strrpos($_FILES['files']['name'][$name], '.'));
  }
  else {
    $filename = $_FILES['files']['name'][$name];
  }

  $file = $filename . $fileext;
  $i = 0;
  while (file_exists($destination . $file)) {
    $i++;
    $file = $filename .'_'. $i . $fileext;
  }
  return $file;
}


function imagepicker_scale_image($source, $destination, $maxsize) {
  $info = image_get_info($source);

  $width = ($maxsize >= $info['width']) ? $info['width'] : $maxsize;
  $height = ($maxsize >= $info['height']) ? $info['height'] : $maxsize;

  $aspect = $info['height'] / $info['width'];
  if ($aspect < $height / $width) {
    $width = (int)min($width, $info['width']);
    $height = (int)round($width * $aspect);
  }
  else {
    $height = (int)min($height, $info['height']);
    $width = (int)round($height / $aspect);
  }

  return image_toolkit_invoke('resize', array($source, $destination, $width, $height));
}

