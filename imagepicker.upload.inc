<?php
// $Id$
// $Name$
// for D7

/**
 * @file
 * upload functions
 */

module_load_include('inc', 'imagepicker', 'imagepicker.imagefuncs');

/**
 * Menu callback; presents the upload form for imagepicker
 */
// iframe
function imagepicker_upload() {

  imagepicker_variable_del('imagepicker_advanced_browser_pagestart');
  if (imagepicker_variable_get('imagepicker_upload_progress_enabled', 1)) {
    $content = imagepicker_upload_progress_get_script(imagepicker_variable_get('imagepicker_upload_progress_delay', 0));
  }
  $content .= imagepicker_quota_ok('iframe', FALSE, '', t('Upload images. You can give them a title and description'));
  return theme('imagepicker_iframe', array('content' => $content));
}
// account
function imagepicker_user_upload() {

  global $user;
  imagepicker_variable_del('imagepicker_advanced_browser_pagestart');
  if (imagepicker_variable_get('imagepicker_upload_progress_enabled', 1)) {
    $content = imagepicker_upload_progress_get_script(imagepicker_variable_get('imagepicker_upload_progress_delay', 0));
  }
  $content .= imagepicker_quota_ok('user', $user, '', t('Upload images. You can give them a title and description'));
  return $content;
}

function imagepicker_upload_form($form, &$form_state, $account=FALSE, $admin = FALSE) {

  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  $form['#attributes'] = array('enctype' => 'multipart/form-data');
  if (imagepicker_variable_get('imagepicker_upload_progress_enabled', 1) && imagepicker_variable_get('imagepicker_uploadprogress_server', '')) {
    $form[IMAGEPICKER_UPLOAD_ID] = array(
      '#type' => 'hidden',
      '#value' => uniqid(),
      '#weight' => -50,
    );
  }
  $max_filesize = ini_get('upload_max_filesize');
  $form['file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Image file'),
    '#description' => t('Browse your computer for an image file, Max. %max', array('%max' => $max_filesize)),
    '#required' => TRUE,
    '#value' => 1,
    '#size' => 40,
  );
  $form['thumb'] = array(
    '#type' => 'textfield',
    '#title' => t('Thumbnail size'),
    '#size' => 10,
    '#default_value' => imagepicker_variable_get('imagepicker_default_thumbnail_size', imagepicker_variable_get('imagepicker_default_thumbnail_size', 100), $user->uid),
    '#description' => t('Size in pixels of thumbnail\'s bigger side'),
    '#required' => TRUE,
  );
  $form['scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Scale image'),
    '#size' => 10,
    '#default_value' => imagepicker_variable_get('imagepicker_default_scale', imagepicker_variable_get('imagepicker_default_scale', ''), $user->uid),
    '#description' => t('Scale image to this size in pixels if not left empty'),
  );
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Add a title for your image'),
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#rows' => 2,
    '#cols' => 80,
    '#description' => t('Add a description for your image'),
  );
  if ( imagepicker_image_check_functions(TRUE) && imagepicker_variable_get('imagepicker_watermark_enable', 0)) {
    if (! imagepicker_variable_get('imagepicker_watermark_image', '')
      && imagepicker_variable_get('imagepicker_watermark_image', '', $user->uid)
      && imagepicker_variable_get('imagepicker_watermark_enable', 0, $user->uid)) {
      $form['watermark'] = array(
        '#type' => 'checkbox',
        '#title' => t('Use watermark'),
        '#description' => t('Use watermark on this image.'),
        '#default_value' => imagepicker_variable_get('imagepicker_watermark_use', 0, $user->uid),
      );
    }
    elseif (imagepicker_variable_get('imagepicker_watermark_image', '')) {
      $form['watermark'] = array(
        '#type' => 'value',
        '#value' => 1,
      );
    }
  }
  // groups
  if (imagepicker_variable_get('imagepicker_groups_enabled', 1) && imagepicker_variable_get('imagepicker_groups_in_upload_enabled', 1)) {
    $grouplist = imagepicker_get_groups( ($admin ? $user : FALSE));
    if ($grouplist) {
      $form['grouplist'] = array(
        '#type' => 'checkboxes',
        '#options' => $grouplist,
        '#title' => t('Your Groups'),
        '#description' => t('Select a group to add your image to.'),
      );
    }
  }

  if ($account) {
    $form['account'] = array(
      '#type' => 'value',
      '#value' => $user->uid,
    );
  }
  if ($admin) {
    $form['admin'] = array(
      '#type' => 'value',
      '#value' => 1,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Upload'),
  );
  if (imagepicker_variable_get('imagepicker_upload_progress_enabled', 1)) {
    $form['#suffix'] = imagepicker_upload_progress_busy();
  }
  return $form;
}

/**
 * Validate form
 */
function imagepicker_upload_form_validate($form, &$form_state) {

  foreach ($form_state['values'] as $name => $value) {
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
    if (isset($form_state['values']['account']) && isset($form_state['values']['admin'])) {
      $user = user_load($form_state['values']['account']);
    }
    else {
      global $user;
    }
    // absolute paths
    $destdir = imagepicker_get_path(FALSE, (isset($form_state['values']['admin']) ? array('name' => $user->name, 'uid' => $user->uid ) : TRUE));
    $thumbsdir = $destdir . IMAGEPICKER_THUMBS_DIR;
    $browserdir = $destdir . IMAGEPICKER_BROWSER_DIR;
    $origdir = $destdir . IMAGEPICKER_ORIG_DIR;

    // relative paths
    $destdirscheme = imagepicker_get_path(FALSE, (isset($form_state['values']['admin']) ? array('name' => $user->name, 'uid' => $user->uid ) : TRUE), TRUE);
    $thumbsdirscheme = $destdirscheme . IMAGEPICKER_THUMBS_DIR . DIRECTORY_SEPARATOR;
    $browserdirscheme = $destdirscheme . IMAGEPICKER_BROWSER_DIR . DIRECTORY_SEPARATOR;
    $origdirscheme = $destdirscheme . IMAGEPICKER_ORIG_DIR . DIRECTORY_SEPARATOR;

    if (file_prepare_directory($destdir, FILE_CREATE_DIRECTORY)
      && file_prepare_directory($thumbsdir, FILE_CREATE_DIRECTORY)
      && file_prepare_directory($browserdir, FILE_CREATE_DIRECTORY)
      && file_prepare_directory($origdir, FILE_CREATE_DIRECTORY)
      ) {
      // clear out the noisy 'created' messages
      drupal_get_messages('status', TRUE);
      // Add DIRECTORY_SEPARATORS here because drupals' functions remove trailing slashes
      $destdir .= DIRECTORY_SEPARATOR;
      $thumbsdir .= DIRECTORY_SEPARATOR;
      $browserdir .= DIRECTORY_SEPARATOR;
      $origdir .= DIRECTORY_SEPARATOR;

      $maxthumbsize = $form_state['values']['thumb'] ? $form_state['values']['thumb'] : 100;
      $scaleto = $form_state['values']['scale'] ? $form_state['values']['scale'] : FALSE;
      $destination = $origdirscheme;

      // save the original and use that from here on
      if ( $file = imagepicker_copy_uploaded_file($destination, 'file_upload') ) {
        // $source is absolute
        $source = $origdir . $file;
        if ($scaleto) {
          $imagescaled = imagepicker_scale_image($source, IMAGEPICKER_FILE_SCHEME . $destdirscheme . $file, $scaleto);
        }
        else {
          // no scaling, save direct from $origdir to $destdir
          file_unmanaged_copy($source, IMAGEPICKER_FILE_SCHEME . $destdirscheme, FILE_EXISTS_RENAME);
        }
      }

      if (!$scaleto && $file || $scaleto && $imagescaled) {
        // if watermark is enabled just apply to destdir image, not orig or the thumbs
        if (isset($form_state['values']['watermark'])) {
          if (! imagepicker_watermark_process($destdir . $file, $user)) {
            drupal_set_message(t('Error while watermarking an uploaded image.'), 'error');
          }
        }

        if (imagepicker_scale_image($source, IMAGEPICKER_FILE_SCHEME . $thumbsdirscheme . $file, $maxthumbsize)) {
          imagepicker_scale_image($source, IMAGEPICKER_FILE_SCHEME . $browserdirscheme . $file, imagepicker_variable_get('imagepicker_default_browser_thumbnail_size', imagepicker_variable_get('imagepicker_default_browser_thumbnail_size', 100), $user->uid));
          $title = htmlspecialchars($form_state['values']['title']);
          $description = htmlspecialchars($form_state['values']['description']);
          $nextimgid = imagepicker_insert_image($user->uid, $file, $title, $description);
          if ($nextimgid) {
            if ( isset($form_state['values']['grouplist']) && is_array($form_state['values']['grouplist']) && imagepicker_variable_get('imagepicker_groups_enabled', 1) && imagepicker_variable_get('imagepicker_groups_in_upload_enabled', 1) ) {
              $query = db_select('imagepicker', 'i');
              $query->fields('i', array('img_id') );
              $query->condition('uid', $user->uid)->condition('img_name', $file);
              $record = $query->execute()->fetchObject();

              foreach ($form_state['values']['grouplist'] AS $gid) {
                $record->gid = $gid;
                imagepicker_insert_group_image($record);
              }
            }
            drupal_set_message(t('Image was successfully uploaded.'));
            if (isset($form_state['values']['admin'])) {
              $outpath = IMAGEPICKER_ADMIN_PATH . '/images/user/' . $user->uid . '/browse';
            }
            elseif (isset($form_state['values']['account'])) {
              $outpath = 'user/' . $user->uid . '/imagepicker/images/browse';
            }
            else {
              $outpath = 'imagepicker/browse/' . $nextimgid;
            }
            drupal_goto($outpath);
          }
          else {
            file_unmanaged_delete($thumbsdir . $file);
            file_unmanaged_delete($browserdir . $file);
            file_unmanaged_delete($origdir . $file);
            file_unmanaged_delete($destdir . $file);
            drupal_set_message(t('Error while saving information to database for uploaded image.'), 'error');
          }
        }
        else {
          drupal_set_message(t('Error while creating a thumbnail for uploaded image.'), 'error');
        }
      }
      else {
        if (!$scaleto && !$file) {
          drupal_set_message(t('Error while moving uploaded file to its destination.'), 'error');
        }
        else {
          drupal_set_message(t('Error while scaling uploaded file.'), 'error');
        }
      }
      file_unmanaged_delete($destdir . $file);
    }
    else {
      drupal_set_message(t('Unable to create a directory structure for your images.'), 'error');
    }
  }
  if ($form_state['values']['admin']) {
    $outpath = IMAGEPICKER_ADMIN_PATH . '/images';
  }
  elseif ($form_state['values']['account']) {
    $outpath = 'user/' . $user->uid . '/imagepicker';
  }
  else {
    $outpath = 'imagepicker';
  }
  drupal_goto($outpath);
}

function imagepicker_copy_uploaded_file($destination, $name) {

  if ($name && isset($_FILES['files']['tmp_name'][$name]) ) {
    $source = $_FILES['files']['tmp_name'][$name];
    $newsrc = file_unmanaged_copy($source, IMAGEPICKER_FILE_SCHEME . $destination, FILE_EXISTS_RENAME);
    if ($newsrc) {
      // Fix bug in drupal's file_copy function which uses '/' instead of
      // DIRECTORY_SEPARATOR for making directories. This causes problems on
      // Windows machines. Not sure if this is still true in D7
      $newsrc = str_replace('/', DIRECTORY_SEPARATOR, $newsrc);

      $file = imagepicker_get_uploaded_file_name($destination, $name);
      $destination .= $file;

      file_unmanaged_move($newsrc, IMAGEPICKER_FILE_SCHEME . $destination);
      return $file;
    }
  }
  return FALSE;
}

function imagepicker_get_uploaded_file_extension($name) {
  if ($name && isset($_FILES['files']['type'][$name]) ) {
    switch ($_FILES['files']['type'][$name]) {
      // For some reason Internet Explorer 7 and later decides to give us
      // non-standard MIME types for jpg and png files, this is supposedly
      // for 'backwards compatibility'. For more details see:
      // http://msdn.microsoft.com/en-us/library/ms775147(VS.85).aspx#_replace

      // Non-standard MIME type:
      case 'image/pjpeg':
      case 'image/jpeg': $fileext = '.jpg'; break;
      case 'image/gif': $fileext = '.gif'; break;
      // Non-standard MIME type:
      case 'image/x-png':
      case 'image/png': $fileext = '.png'; break;
      default: $fileext = '';
    }
    return $fileext;
  }
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
    $file = $filename . '_' . $i . $fileext;
  }
  return $file;
}

function imagepicker_upload_progress_get_script($delay) {

  $message = imagepicker_variable_get('imagepicker_upload_progress_message', t('Processing form... please wait.'));
  if (imagepicker_variable_get('imagepicker_uploadprogress_server', '')) {
    // Immediately disable devel shutdown functions so that it doesn't botch our
    // JSON output.
    $GLOBALS['devel_shutdown'] = FALSE;
    $settings = array(
      'imagepicker_upload_progress' => array(
        'callback' => 'http://' . $_SERVER['HTTP_HOST'] . url('imagepicker/progress'),
        'interval' => imagepicker_variable_get('imagepicker_uploadprogress_interval', 1),
        'name' => IMAGEPICKER_UPLOAD_ID,
        'delay' => $delay,
        'initmessage' => $message,
        'cancelmessage' => t('Cancel'),
      ),
    );
  }
  else {
    $settings = array(
      'imagepicker_upload_progress' => array(
        'callback' => '',
        'interval' => '',
        'name' => '',
        'delay' => $delay,
        'initmessage' => $message,
        'cancelmessage' => t('Cancel'),
      ),
    );
  }

  drupal_add_js(IMAGEPICKER_PATH .'/imagepicker_upload.js');
  drupal_add_js($settings, 'setting');
}

function imagepicker_upload_progress_busy() {
  return '<div id="imagepicker-sending" style="display: none;"></div>';
}

