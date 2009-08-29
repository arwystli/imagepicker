<?php
// $Id$
// $Name$

/**
 * @file
 * my imagepicker in my account
 */
function imagepicker_user_page($uid, $path="", $func="", $id=0) {
  drupal_add_css(drupal_get_path('module', 'imagepicker') .'/imagepicker.css');

  // path/func/id
  if ($path == 'images') {
    if ( ($func == 'browse' || $func == 'browseadmin') && is_numeric($id) && $id > 0) {
      $content .= imagepicker_user_view($id);
    }
    elseif ($func == 'edit' && is_numeric($id) && $id > 0) {
      include_once('imagepicker.edit.inc');
      $content .= imagepicker_user_image_edit($id);
    }
    elseif ($func == 'browse') {
      $content .= imagepicker_user_browse();
    }
    elseif ($func == 'browse_public') {
      if (is_numeric($id) && $id > 0) {
        $content .= imagepicker_user_view_public($id);
      }
      else {
        $content .= imagepicker_user_browse_public();
      }
    }
    elseif ($func == 'browseadmin') {
      $content .= imagepicker_user_browse_admin();
    }
    elseif ($func == 'delete' && is_numeric($id) && $id > 0) {
      imagepicker_image_delete($id, FALSE, 'account');
    }
  }
  elseif ($path == 'groups') {
    include_once('imagepicker.group.inc');
    if ($func == 'edit' && is_numeric($id) && $id > 0) {
      $content .= imagepicker_user_groups($func, $id);
    }
    elseif ($func == 'browse' && is_numeric($id) && $id > 0) {
      imagepicker_set_user_group_state(1, $id);
      $content .= imagepicker_user_browse();
    }
    elseif ($func == 'browse') {
      $content .= imagepicker_user_groups();
    }
    elseif ($func == 'delete') {
      $content .= drupal_get_form('imagepicker_group_delete_form', $id);
    }
  }
  elseif ($path == 'stats') {
    $content .= imagepicker_group_stats(-1);
  }
  elseif ($path == 'config') {
    $content .= imagepicker_user_config_admin();
  }
  else {
    include_once('imagepicker.upload.inc');
    $content .= imagepicker_user_upload();
  }
  return $content;
}

/**
 * main thumbnails page in my imagepicker
 */
function imagepicker_user_browse() {
  $content = _imagepicker_browse("account");
  return $content;
}

function imagepicker_user_browse_public() {
  $content = _imagepicker_browse_public("account", 1);
  return $content;
}

function imagepicker_user_browse_admin() {
  $content = _imagepicker_browse_admin("account");
  return $content;
}

function imagepicker_user_view($img_id) {

  $img = _imagepicker_get_img($img_id);
  if ($img) {
    $imgpath = imagepicker_get_image_path($img, 'full');
    $info = image_get_info(imagepicker_get_path(FALSE, TRUE) . $img['img_name']);
    $content = theme('imagepicker_view', $img, $imgpath, $info) .
    drupal_get_form('imagepicker_user_image_form', $img_id) .
    '<br />'.
    (_imagepicker_has_groups() ? drupal_get_form('imagepicker_group_images_form', $img['img_id']) : '');
  }
  else {
    drupal_set_message(t('Image not found.'), 'error');
    $content = '';
  }
  return $content;
}

function imagepicker_user_view_public($img_id) {

  $img = _imagepicker_get_img($img_id, FALSE);
  if ($img) {

    $result = db_query("SELECT name FROM {users} WHERE uid=%d AND status=1", array($img['uid']));
    $row = db_fetch_array($result);
    $img['name'] = ($row['name'] ? $row['name'] : FALSE);

    $imgpath = imagepicker_get_image_path($img, 'full', array('name' => $img['name'] , 'uid' => $img['uid'] ));
    $info = image_get_info(imagepicker_get_path(FALSE, array('name' => $img['name'] , 'uid' => $img['uid'] )) . $img['img_name']);
    $content = theme('imagepicker_view', $img, $imgpath, $info);
  }
  else {
    drupal_set_message(t('Image not found.'), 'error');
    $content = '';
  }
  return $content;
}

function imagepicker_user_image_form(&$form_state, $img_id) {
  $form['edit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit image'),
    '#submit' => array('imagepicker_user_image_form_edit'),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete image'),
    '#submit' => array('imagepicker_user_image_form_delete'),
  );
  $form['img_id'] = array(
    '#type' => 'hidden',
    '#value' => $img_id,
  );
  return $form;
}

function imagepicker_user_image_form_delete($form, &$form_state) {
  imagepicker_image_delete($form_state['values']['img_id'] , FALSE, 'account');
}

function imagepicker_user_image_form_edit($form, &$form_state) {
  global $user;
  drupal_goto('user/'. $user->uid .'/imagepicker/images/edit/'. $form_state['values']['img_id']);
}

/**
 * Menu callback; presents the configuration settings form for imagepicker user.
 */
function imagepicker_user_config_admin() {
  $content = drupal_get_form('imagepicker_user_config_admin_form');
  return theme('imagepicker_user_config', $content, '', t('Imagepicker configuration page.'), '', '');
}

/**
 * Submit form
 */
function imagepicker_user_config_admin_form(&$form_state, $account = FALSE) {
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  $form['imagepicker_advanced_browser_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Images per page'),
    '#description' => t('Configure the number of images shown per page in the image browser. This must be a number, 0 for no paging'),
    '#size' => 4,
    '#required' => FALSE,
    '#default_value' => ($user->imagepicker_advanced_browser_page ? $user->imagepicker_advanced_browser_page : variable_get('imagepicker_advanced_browser_page', 25)),
  );

  if ( imagepicker_image_check_functions()
    && variable_get('imagepicker_watermark_enable', 0)
    && ! variable_get('imagepicker_watermark_image', '')
  ) {

  $desc = "";
  $wfiles = imagepicker_get_watermarks($user);
  if ($wfiles) {
    foreach ($wfiles AS $k => $v) {
      $wf[$v] = "$v";
    }
  }
  else {
    $desc = t('You have no uploaded watermark images');
  }

  $form['#attributes']['enctype'] = 'multipart/form-data';


  $form['imagepicker_watermark'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image watermark settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['imagepicker_watermark']['imagepicker_watermark_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable watermark'),
    '#description' => $desc,
    '#default_value' => ($user->imagepicker_watermark_enable ? $user->imagepicker_watermark_enable : FALSE),
    '#return_value' => 1,
  );

  if ($wf) {
    $form['imagepicker_watermark']['imagepicker_watermark_image'] = array(
      '#type' => 'select',
      '#default_value' => ($user->imagepicker_watermark_image ? $user->imagepicker_watermark_image : ''),
      '#options' => $wf,
      '#title' => t('Select Watermark image'),
      '#prefix' => '<div class="container-inline">',
    );
    $form['imagepicker_watermark']['imagepicker_watermark_image_delete'] = array(
      '#type' => 'submit',
      '#value' => t('Delete'),
      '#submit' => array('imagepicker_user_config_admin_form_delete'),
      '#suffix' => '</div>'
    );
  }
  $form['imagepicker_watermark']['file_upload'] = array(
    '#type' => 'file',
    '#title' => t('Upload Watermark Image'),
    '#description' => t('Browse your computer for watermark image file'),
    '#required' => FALSE,
    '#value' => 1,
    '#size' => 40,
  );
  $form['imagepicker_watermark']['imagepicker_watermark_position'] = array(
    '#type' => 'select',
    '#default_value' => ($user->imagepicker_watermark_position ? $user->imagepicker_watermark_position : 0),
    '#options' =>  imagepicker_watermark_opts(),
    '#title' => t('Select watermark position'),
  );
  $form['imagepicker_watermark']['imagepicker_watermark_use'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use watermark by default'),
    '#description' => t('Check the "Use watermark" checkbox on the upload form by default'),
    '#default_value' => ($user->imagepicker_watermark_use ? $user->imagepicker_watermark_use : FALSE),
    '#return_value' => 1,
  );

  // watermark scaling
  $form['imagepicker_watermark']['imagepicker_user_watermark'] = array(
    '#type' => 'fieldset',
    '#title' => t('Watermark scaling'),
    '#collapsible' => TRUE,
    '#collapsed' => ($user->imagepicker_watermark_scaling ? TRUE : FALSE),
  );
  $form['imagepicker_watermark']['imagepicker_user_watermark']['imagepicker_watermark_scaling'] = array(
    '#type' => 'checkbox',
    '#title' => t('Scale watermark'),
    '#description' => t('Use scaling, if you want the watermark to be proportional to the image width. Please note that the watermark is scaled only when it is too big.'),
    '#default_value' => ($user->imagepicker_watermark_scaling ? $user->imagepicker_watermark_scaling : FALSE),
    '#return_value' => 1,
  );
  $form['imagepicker_watermark']['imagepicker_user_watermark']['imagepicker_watermark_scaling_percentage'] = array(
    '#type' => 'textfield',
    '#title' => t('Percentage'),
    '#description' => t('Percentage to apply the watermark to the image.'),
    '#size' => 3,
    '#default_value' => ($user->imagepicker_watermark_scaling_percentage ? $user->imagepicker_watermark_scaling_percentage : 50),
  );
  $form['imagepicker_watermark']['imagepicker_user_watermark']['imagepicker_watermark_min_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Minimum width'),
    '#description' => t('The mimimum of the watermark in pixels. Type 0 to turn this option off.'),
    '#size' => 3,
    '#default_value' => ($user->imagepicker_watermark_min_width ? $user->imagepicker_watermark_min_width : 0),
  );
  }
  if ($account) {
    $form['uid'] = array(
      '#type' => 'hidden',
      '#value' => $user->uid,
    );
  }
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
  );
  return $form;
}
function imagepicker_user_config_admin_form_validate($form, &$form_state) {
  if ($form_state['values']['uid']) {
    $user = user_load(array('uid' => $form_state['values']['uid']));
  }
  else {
    global $user;
  }

  if (! is_numeric($form_state['values']['imagepicker_advanced_browser_page']) && $form_state['values']['imagepicker_advanced_browser_page'] < 0) {
    form_set_error('imagepicker_advanced_browser_page', t('The browser page must be a positive number or zero'));
  }

  // also used by the imagepicker admin form
  if ( $form_state['values']['imagepicker_watermark_image'] ) {
    $wdir = imagepicker_get_watermarks_dir($user);
    $file = $wdir . DIRECTORY_SEPARATOR . $form_state['values']['imagepicker_watermark_image'];
    if (! file_exists($file)) {
      form_set_error('imagepicker_watermark_image', t('This file does not exist or has invalid path: %file.', array('%file' => $file)));
    }
    if ($form_state['values']['imagepicker_watermark_scaling']) {
      if (! is_numeric($form_state['values']['imagepicker_watermark_scaling_percentage']) || $form_state['values']['imagepicker_watermark_scaling_percentage'] <= 0 || $form_state['values']['imagepicker_watermark_scaling_percentage'] >= 100) {
        form_set_error('imagepicker_watermark_scaling_percentage', t('Please enter a number between 1 and 100 for scaling percentage.'));
      }
      elseif ($form_state['values']['imagepicker_watermark_scaling_percentage'] < 10 || $form_state['values']['imagepicker_watermark_min_width'] < 50) {
        // we do not disable scaling for low values, e.g. if the user chooses to scale down to 1%, we just warn
        drupal_set_message(t('Warning: Scaling is set to @percentage%, minimum width is only @min_width px. This may result in very small or almost invisible watermarks. It is recommended to check twice that an adequate minimum width has been set.',
          array('@percentage' => $form_state['values']['imagepicker_watermark_scaling_percentage'], '@min_width' => $form_state['values']['imagepicker_watermark_min_width'])));
      }
      if (intval($form_state['values']['imagepicker_watermark_min_width']) < 0) {
        form_set_error('imagepicker_watermark_min_width', t('Please enter a positive number for minimum width.'));
      }
    }
  }

  if (! empty($_FILES['files']['name']['file_upload'])) {
    include_once('imagepicker.upload.inc');
    if (!isset($_FILES['files']['tmp_name']['file_upload']) || !file_exists($_FILES['files']['tmp_name']['file_upload'])) {
      form_set_error('file_upload', t('Error while uploading file.'));
    }
    elseif (!image_get_info($_FILES['files']['tmp_name']['file_upload'])) {
      form_set_error('file_upload', t('Uploaded file is not an image.'));
    }
    elseif (!imagepicker_get_uploaded_file_extension('file_upload')) {
      form_set_error('file_upload', t('Only .jpg, .gif and .png image files are accepted.'));
    }
  }
}

function imagepicker_user_config_admin_form_submit($form, &$form_state) {
  if ( $form_state['values']['uid'] ) {
    $user = user_load(array('uid' => $form_state['values']['uid']));
  }
  else {
    global $user;
  }

  if (! empty($_FILES['files']['name']['file_upload'])) {
    include_once('imagepicker.upload.inc');
    $wdir = imagepicker_get_watermarks_dir();
    if (file_check_directory($wdir, TRUE)) {
      // clear out the noisy 'created' messages
      drupal_get_messages('status', TRUE);
      $wdir = $wdir . DIRECTORY_SEPARATOR;
      // $destination gets altered in imagepicker_copy_uploaded_file to give us the final path + name
      $destination = $wdir;
      // save the original and use that from here on
      if ( $imagemoved = imagepicker_copy_uploaded_file($destination, 'file_upload') ) {
        // $destination has been changed in imagepicker_copy_uploaded_file()
        // to point to the new file
        $file = basename($destination);
        $source = $wdir . $file;
      }
    }
  }
  // got one
  if ($file) {
    $form_state['values']['imagepicker_watermark_image'] = $file;
  }
  // save all
  if ($form_state['values']['imagepicker_watermark_image']) {
    user_save($user, array('imagepicker_watermark_enable' => $form_state['values']['imagepicker_watermark_enable']));
    user_save($user, array('imagepicker_watermark_image' => $form_state['values']['imagepicker_watermark_image']));
    user_save($user, array('imagepicker_watermark_position' => $form_state['values']['imagepicker_watermark_position']));
    user_save($user, array('imagepicker_watermark_use' => $form_state['values']['imagepicker_watermark_use']));
    if ($form_state['values']['imagepicker_watermark_scaling']) {
      user_save($user, array('imagepicker_watermark_scaling' => $form_state['values']['imagepicker_watermark_scaling'] ));
      user_save($user, array('imagepicker_watermark_scaling_percentage' => $form_state['values']['imagepicker_watermark_scaling_percentage']));
      user_save($user, array('imagepicker_watermark_min_width' => $form_state['values']['imagepicker_watermark_min_width']));
    }
  }
  else {
    user_save($user, array('imagepicker_watermark_image' => NULL));
  }

  user_save($user, array('imagepicker_advanced_browser_page' => $form_state['values']['imagepicker_advanced_browser_page']));
  drupal_set_message(t('Config settings saved'));
}

function imagepicker_user_config_admin_form_delete($form, &$form_state) {
  if ( $form_state['values']['uid'] ) {
    $user = user_load(array('uid' => $form_state['values']['uid']));
  }
  else {
    global $user;
  }
  if ($form_state['values']['imagepicker_watermark_image']) {
    $file = imagepicker_get_watermarks_dir() . DIRECTORY_SEPARATOR . $form_state['values']['imagepicker_watermark_image'];
    if (file_exists($file)) {
      file_delete($file);
    }
    user_save($user, array('imagepicker_watermark_image' => NULL));
    drupal_set_message(t('Watermark File deleted'));
  }
}

// not in use I think
/**
 * Helper function to check if a user has a specific role
 */
function imagepicker_user_has_role($role, $user=NULL) {
  if ($user == NULL) {
    global $user;
  }

  if (is_array($user->roles) && in_array($role, array_values($user->roles))) {
    return TRUE;
  }

  return FALSE;
}
