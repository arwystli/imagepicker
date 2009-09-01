<?php
// $Id$
// $Name$

/**
 * @file
 * admin settings functions
 */

/**
 * Menu callback; presents the settings form for imagepicker
 */
function imagepicker_settings_page($op = NULL) {
  drupal_add_css(drupal_get_path('module', 'imagepicker') .'/imagepicker.css');
  $content = drupal_get_form('imagepicker_settings_form');
  return $content;
}

/**
 * Function to display the imagepicker admin settings form
 * @return
 *   Returns the form.
 */
function imagepicker_settings_form($form_state) {

  // default settings
  $form['imagepicker_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set imagepicker settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['imagepicker_settings']['imagepicker_default_browser_thumbnail_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Browser Thumbnail Size'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_default_browser_thumbnail_size', 100),
    '#description' => t('Configure the default browser thumbnail size'),
  );
  $node_types = node_get_types('names');
  $form['imagepicker_settings']['imagepicker_node_types_enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Set the node types you want to enable Imagepicker for.'),
    '#default_value' => variable_get('imagepicker_node_types_enabled', array_keys($node_types)),
    '#options' => $node_types,
  );
  if (module_exists('comment')) {
    $form['imagepicker_settings']['imagepicker_comment_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comments'),
      '#return_value' => 1,
      '#default_value' => variable_get('imagepicker_comment_enabled', 0),
      '#description' => t('Setting this option enables Imagepicker in comments.'),
    );
  }
  $form['imagepicker_settings']['imagepicker_blocks_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Blocks'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_blocks_enabled', 0),
    '#description' => t('Setting this option enables Imagepicker in blocks.'),
  );
  $form['imagepicker_settings']['imagepicker_default_pagelink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show return link in page'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_default_pagelink', 1),
    '#description' => t('Setting this option will add a link back to the thumbnail when using the page option. Uses javascript history(back).'),
  );
  $form['imagepicker_settings']['imagepicker_account_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Imagepicker in My Account'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_account_enabled', 1),
    '#description' => t('Setting this option enables Imagepicker in My Account.'),
  );
  // default options
  $form['imagepicker_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set imagepicker defaults'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['imagepicker_defaults']['imagepicker_default_thumbnail_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Thumbnail Size'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_default_thumbnail_size', 100),
    '#description' => t('Configure the default thumbnail size'),
  );
  $form['imagepicker_defaults']['imagepicker_default_scale'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Scale'),
    '#size' => 10,
    '#required' => FALSE,
    '#default_value' => variable_get('imagepicker_default_scale', ''),
    '#description' => t('Configure the default scale. leave empty for no default scale'),
  );
  // insert settings
  $form['imagepicker_insert_defaults'] = array(
    '#type' => 'fieldset',
    '#title' => t('Set imagepicker insert defaults'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
    '#description' => ''
  );
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_align'] = imagepicker_get_align_opts();
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_show'] = imagepicker_get_show_opts();
  $form['imagepicker_insert_defaults']['imagepicker_insert_defaults_link'] = imagepicker_get_link_opts();

  // alignment settings
  $form['imagepicker_align'] = array(
    '#type' => 'fieldset',
    '#title' => t('Image alignment settings'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['imagepicker_align']['imagepicker_default_align_show'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show Alignment options'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_default_align_show', 1),
    '#description' => t('Unsetting this option will remove the alignment options from the insert page.'),
  );
  $form['imagepicker_align']['imagepicker_default_fleft'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Float left code'),
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_default_fleft', 'style="float: left"'),
    '#description' => t('Configure the default code used for float left styling.'),
  );
  $form['imagepicker_align']['imagepicker_default_fright'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Float right code'),
    '#size' => 25,
    '#maxlength' => 50,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_default_fright', 'style="float: right"'),
    '#description' => t('Configure the default code used for float right styling.'),
  );

  if (module_exists('lightbox2') || module_exists('thickbox')) {
    $form['imagepicker_module'] = array(
      '#type' => 'fieldset',
      '#title' => t('Module integration'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    if (module_exists('lightbox2')) {
      $form['imagepicker_module']['imagepicker_lightbox2_enable'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable Lightbox2 in Imagepicker'),
        '#return_value' => 1,
        '#default_value' => variable_get('imagepicker_lightbox2_enable', 1),
        '#description' => t('Unsetting this option will disable Lightbox2 in Imagepicker.'),
      );
      $form['imagepicker_module']['imagepicker_lightbox2_insert'] = array(
        '#type' => 'textfield',
        '#title' => t('Default Lightbox insert'),
        '#size' => 15,
        '#maxlength' => 30,
        '#required' => TRUE,
        '#default_value' => variable_get('imagepicker_lightbox2_insert', 'lightbox'),
        '#description' => t('Configure the default code inserted into the "rel" attribute.'),
      );
    }
    if (module_exists('thickbox')) {
      $form['imagepicker_module']['imagepicker_thickbox_enable'] = array(
        '#type' => 'checkbox',
        '#title' => t('Enable Thickbox in Imagepicker'),
        '#return_value' => 1,
        '#default_value' => variable_get('imagepicker_thickbox_enable', 1),
        '#description' => t('Unsetting this option will disable Thickbox in Imagepicker.'),
      );
    }
  }
  // quota
  $form['imagepicker_quotas'] = array(
    '#type' => 'fieldset',
    '#title' => t('User quotas'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['imagepicker_quotas']['imagepicker_quota_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Quotas in Imagepicker'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_quota_enable', 1),
    '#description' => t('Unsetting this option will disable Quotas in Imagepicker.'),
  );
  $form['imagepicker_quotas']['imagepicker_quota_default'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('imagepicker_quota_default', 0),
    '#options' => imagepicker_get_quota_list(),
    '#title' => t('Set default quota'),
  );
  if ( imagepicker_image_check_functions() ) {
    // advanced settings
    $form['imagepicker_watermark'] = array(
      '#type' => 'fieldset',
      '#title' => t('Image watermark settings'),
      '#collapsible' => TRUE,
      '#collapsed' => ( variable_get('imagepicker_watermark_enable', 0) ? FALSE : TRUE),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_enable'] = array(
      '#type' => 'checkbox',
      '#title' => t('Enable Watermarks in Imagepicker'),
      '#return_value' => 1,
      '#default_value' => variable_get('imagepicker_watermark_enable', 0),
      '#description' => t('Setting this option will enable Watermarks on new images in Imagepicker.'),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_image'] = array(
      '#type' => 'textfield',
      '#title' => t('Default Watermark image'),
      '#size' => 55,
      '#maxlength' => 100,
      '#required' => FALSE,
      '#default_value' => variable_get('imagepicker_watermark_image', ''),
      '#description' => t('Path of the image that should be used as the watermark image, relative to the Drupal root.  <br />This should be a truecolor (32 bit) PNG file with an alpha channel, but other formats may work.<br />If this field is used, the watermark will be applied to all new images uploaded to the site.<br />If this field is left empty users with access will have the option to apply their own watermark.'),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_position'] = array(
      '#type' => 'select',
      '#default_value' => variable_get('imagepicker_watermark_position', 0),
      '#options' => imagepicker_watermark_opts(),
      '#title' => t('Select default watermark position'),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_scale'] = array(
      '#type' => 'fieldset',
      '#title' => t('Watermark scaling'),
      '#collapsible' => TRUE,
      '#collapsed' => FALSE,
    );
    $form['imagepicker_watermark']['imagepicker_watermark_scale']['imagepicker_watermark_scaling'] = array(
      '#type' => 'checkbox',
      '#title' => t('Scale watermark'),
      '#description' => t('Use scaling, if you want the watermark to be proportional to the image width. Please note that the watermark is scaled only when it is too big.'),
      '#default_value' => variable_get('imagepicker_watermark_scaling', FALSE),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_scale']['imagepicker_watermark_scaling_percentage'] = array(
      '#type' => 'textfield',
      '#title' => t('Percentage'),
      '#description' => t('Percentage to apply the watermark to the image.'),
      '#size' => 3,
      '#default_value' => variable_get('imagepicker_watermark_scaling_percentage', 50),
    );
    $form['imagepicker_watermark']['imagepicker_watermark_scale']['imagepicker_watermark_min_width'] = array(
      '#type' => 'textfield',
      '#title' => t('Minimum width'),
      '#description' => t('The mimimum of the watermark in pixels. Type 0 to turn this option off.'),
      '#size' => 3,
      '#default_value' => variable_get('imagepicker_watermark_min_width', 0),
    );
  }
  // advanced settings
  $form['imagepicker_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced imagepicker settings'),
    '#description' => t('You can alter the default styling of the iframe here,<br />useful if the iframe is not fitting in with your theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  if (module_exists('thickbox') && file_exists(drupal_get_path('module', 'imagepicker') .'/imagepicker_thickbox.js')) {
    $form['imagepicker_advanced']['imagepicker_advanced_thickbox'] = array(
      '#type' => 'checkbox',
      '#title' => t('Thickbox Iframe'),
      '#return_value' => 1,
      '#default_value' => variable_get('imagepicker_advanced_thickbox', 0),
      '#description' => t('Use thickbox iframe instead of the builtin one. See the contribs folder in Imagepicker'),
    );
  }
  $form['imagepicker_advanced']['imagepicker_advanced_iframe_width'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe width'),
    '#description' => t('Configure the iframe width. This can be a number or a percentage, eg 400 or 100%'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_advanced_iframe_width', "100%"),
  );
  $form['imagepicker_advanced']['imagepicker_advanced_iframe_height'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe height'),
    '#description' => t('Configure the iframe height. This must be a number, eg 500'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_advanced_iframe_height', "500"),
  );
  $form['imagepicker_advanced']['imagepicker_advanced_iframe_border'] = array(
    '#type' => 'textfield',
    '#title' => t('Iframe border'),
    '#description' => t('Configure the iframe border. This can be a number, eg 0 or 1, or a css entry such as 1px solid #808080'),
    '#size' => 25,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_advanced_iframe_border', "0"),
  );
  $form['imagepicker_advanced']['imagepicker_advanced_browser_columns'] = array(
    '#type' => 'textfield',
    '#title' => t('Browser columns'),
    '#description' => t('Configure the number of columns in the image browser. This must be a number, 0 for normal wraparound'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_advanced_browser_columns', 0),
  );
  $form['imagepicker_advanced']['imagepicker_advanced_browser_page'] = array(
    '#type' => 'textfield',
    '#title' => t('Images per page'),
    '#description' => t('Configure the number of images shown per page in the image browser. This must be a number, 0 for no paging'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_advanced_browser_page', 20),
  );
  $form['imagepicker_advanced']['imagepicker_advanced_collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fieldset state'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_advanced_collapsed', 0),
    '#description' => t('Setting this option will collapse the fieldset the iframe is in by default.'),
  );
  $form['imagepicker_advanced']['imagepicker_use_full_url'] = array(
    '#type' => 'checkbox',
    '#title' => t('Use full url'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_use_full_url', 1),
    '#description' => t('Setting this option will make imagepicker use a full url to the image being inserted, unsetting it will use an absolute path.<br />This is useful if you are developing a site with a different url than the production site will be on.'),
  );
  $form['imagepicker_advanced']['imagepicker_use_cssbox'] = array(
    '#type' => 'checkbox',
    '#title' => t('Allow additional css'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_use_cssbox', 0),
    '#description' => t('Setting this option will provide a textfield where additional css can be added .'),
  );
  $form['imagepicker_advanced']['imagepicker_show_browse_order_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the order select box in the browser'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_show_browse_order_form', 1),
    '#description' => t(''),
  );
  $orderlist = array(
    'img_id DESC' => t('Newest first'),
    'img_id ASC' => t('Newest last'),
    'img_date DESC' => t('Edited first'),
    'img_date ASC' => t('Edited last'),
    'img_name' => t('By name'),
  );
  $form['imagepicker_advanced']['imagepicker_default_browser_order'] = array(
    '#type' => 'select',
    '#default_value' => variable_get('imagepicker_default_browser_order', 'img_id DESC'),
    '#options' => $orderlist,
    '#title' => t('Default order'),
    '#description' => t('The default order used to sort the browser. This will be applied wether or not the select box is visible'),
  );
  $form['imagepicker_advanced']['imagepicker_show_browse_search_form'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show the search form in the browser'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_show_browse_search_form', 1),
    '#description' => t(''),
  );
  $form['imagepicker_advanced']['imagepicker_public_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Public option'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_public_enabled', 1),
    '#description' => t('Allow users to make image groups publicly available.'),
  );
  $form['imagepicker_advanced']['imagepicker_publicroles_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Public Role Control option'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_publicroles_enabled', 1),
    '#description' => t('Allow users to make publicly available image groups restricted by role.'),
  );
  $form['imagepicker_advanced']['imagepicker_groups_in_upload_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Groups in upload form '),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_groups_in_upload_enabled', 1),
    '#description' => t('Allow users to select a group in the upload form.'),
  );

  $form['imagepicker_advanced']['imagepicker_user_config_enable'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable User config form'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_user_config_enable', 1),
    '#description' => t('Allow users to select their own settings.'),
  );

  $form['imagepicker_advanced']['imagepicker_upload_progress_enabled'] = array(
    '#type' => 'checkbox',
    '#title' => t('Enable Progress bar in upload'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_upload_progress_enabled', 1),
    '#description' => t(''),
  );
  $form['imagepicker_advanced']['imagepicker_upload_progress_delay'] = array(
    '#type' => 'textfield',
    '#title' => t('Progress bar Delay'),
    '#size' => 3,
    '#default_value' => variable_get('imagepicker_upload_progress_delay', 3),
    '#field_suffix' => t('seconds'),
    '#description' => t('Show the progress bar if processing is longer than this value.'),
  );
  $form['imagepicker_advanced']['imagepicker_upload_progress_message'] = array(
    '#type' => 'textfield',
    '#title' => t('Progress bar message'),
    '#size' => 60,
    '#default_value' => variable_get('imagepicker_upload_progress_message', t('Processing form... please wait.')),
    '#description' => t('Message to display while uploading.'),
  );

  return system_settings_form($form);
}

/**
 * Validate settings form
 */
function imagepicker_settings_form_validate($form, &$form_state) {
  global $user;
  if (! drupal_strlen($form_state['values']['imagepicker_default_thumbnail_size'])) {
    form_set_error('imagepicker_default_thumbnail_size', t('You must fill in the Thumbnail field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_default_thumbnail_size'])) {
    form_set_error('imagepicker_default_thumbnail_size', t('The Thumbnail field must be a number'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_default_browser_thumbnail_size'])) {
    form_set_error('imagepicker_default_browser_thumbnail_size', t('You must fill in the Browser Thumbnail field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_default_browser_thumbnail_size'])) {
    form_set_error('imagepicker_default_browser_thumbnail_size', t('The Browser Thumbnail field must be a number'));
  }
  if (drupal_strlen($form_state['values']['imagepicker_default_scale']) && ! is_numeric($form_state['values']['imagepicker_default_scale'])) {
    form_set_error('imagepicker_default_scale', t('The Scale field must be a number'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_default_fleft'])) {
    form_set_error('imagepicker_default_fleft', t('You must fill in the Left alignment field'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_default_fright'])) {
    form_set_error('imagepicker_default_fright', t('You must fill in the Right alignment field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_iframe_height'])) {
    form_set_error('imagepicker_advanced_iframe_height', t('The Iframe height must be a number'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_advanced_iframe_width'])) {
    form_set_error('imagepicker_advanced_iframe_width', t('You must fill in the Iframe width'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_advanced_iframe_border'])) {
    form_set_error('imagepicker_advanced_iframe_border', t('You must fill in the Iframe border'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_browser_columns']) && $form_state['values']['imagepicker_advanced_browser_columns'] < 0) {
    form_set_error('imagepicker_advanced_browser_columns', t('The browser columns must be a positive number or zero'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_browser_page']) && $form_state['values']['imagepicker_advanced_browser_page'] < 0) {
    form_set_error('imagepicker_advanced_browser_page', t('The browser page must be a positive number or zero'));
  }
  if (! is_numeric($form_state['values']['imagepicker_upload_progress_delay']) && $form_state['values']['imagepicker_upload_progress_delay'] < 0) {
    form_set_error('imagepicker_upload_progress_delay', t('The progress delay must be a positive number or zero'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_insert_defaults_align'])) {
    form_set_error('imagepicker_insert_defaults_align', t('You must fill in the default align field'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_insert_defaults_show'])) {
    form_set_error('imagepicker_insert_defaults_show', t('You must fill in the default show field'));
  }
  if (! drupal_strlen($form_state['values']['imagepicker_insert_defaults_link'])) {
    form_set_error('imagepicker_insert_defaults_link', t('You must fill in the default link field'));
  }
  if ( imagepicker_image_check_functions() && $form_state['values']['imagepicker_watermark_enable'] ) {
    include_once('imagepicker.user.inc');
    imagepicker_user_config_admin_form_validate($form, $form_state);
  }

  if ($form_state['values']['imagepicker_show_browse_order_form'] != 1) {
    user_save($user, array('imagepicker_browser_order' => NULL));
  }
}

/**
 * Administration functions
 */
function imagepicker_admin_users_list($dir, $label='') {

  $how_many = variable_get('imagepicker_rows_per_page', 25);
  $header = array(
    array('data' => t('Name'), 'field' => 'u.name'),
    t('Images'),
    t('Groups'),
    (variable_get('imagepicker_quota_enable', 1) ? t('Quotas') : ''),
  );
  $rowcount = 0;
  $result = db_query("
    SELECT DISTINCT u.uid, u.name
    FROM {imagepicker} i, {users} u
    WHERE i.uid=u.uid AND status=1".  tablesort_sql($header));
  while ($row = db_fetch_array($result)) {
    $user = user_load( array('uid' => $row['uid']));
    $allcount = _imagepicker_user_has_img($user);
    $groupcount = _imagepicker_user_has_groups('all', $user);
    $quota = isset($user->imagepicker_quota) ? $user->imagepicker_quota : variable_get('imagepicker_quota_default', 0);

    $row_data = array(
      check_plain($row['name']),
      ($allcount ? $allcount .' '. l(t('images'), 'admin/settings/imagepicker/images/user/'. $row['uid'] .'/browse' ) : t('No images')),
      ($groupcount ? $groupcount .' '. l(t('groups'), 'admin/settings/imagepicker/groups/user/'. $row['uid'] .'/browse' ) : t('No groups')),
      (variable_get('imagepicker_quota_enable', 1) ? l(imagepicker_get_quota_list($quota+1), 'admin/settings/imagepicker/images/user/'. $row['uid'] .'/quota') : ''),
    );
    $rows[] = $row_data;
    $rowcount++;
  }
  return theme('imagepicker_list', $header, $rows, $how_many, t('No users found.'), '', '', $label);
}

function imagepicker_admin_groups_list($label="") {
  $how_many = variable_get('imagepicker_rows_per_page', 25);
    $header = array(
      array('data' => t('Group Name'), 'field' => 'g.group_name'),
      array('data' => t('User Name'), 'field' => 'u.name'),
      array('data' => t('Status'), 'field' => 'g.public'),
      array('data' => t('Images'), 'field' => 'ct'),
      array('data' => t('Actions'), 'colspan' => 2),
    );
  $sql = "SELECT g.gid, g.uid, g.group_name, g.group_description, g.state, g.public, COUNT(i.img_id) AS ct, u.name
    FROM {imagepicker_user_groups} g LEFT JOIN {imagepicker_group_images} i USING(gid), {users} u
    WHERE g.uid=u.uid
    GROUP BY g.gid". tablesort_sql($header);
  $result = db_query($sql);
  $rowcount = 0;
  while ($row = db_fetch_array($result)) {
    $row_data = array(
      $row['group_name'],
      l($row['name'], 'admin/settings/imagepicker/groups/user/'. $row['uid']),
      ($row['public'] ? t('Public') : t('Private')),
      ($row['ct'] ? l(format_plural($row['ct'], '1 image', '@count images'), 'admin/settings/imagepicker/groups/user/'. $row['uid'] .'/browse/'. $row['gid'] ) : t('No images')),
      l(t('Edit'), 'admin/settings/imagepicker/groups/user/'. $row['uid'] .'/edit/'. $row['gid']) ,
      l(t('Delete'), 'admin/settings/imagepicker/groups/user/'. $row['uid'] .'/delete/'. $row['gid']) ,
    );
    $rows[] = $row_data;
    $rowcount++;
  }
  $pref = '';
  $suff = '';
  return theme('imagepicker_list', $header, $rows, $how_many, t('No groups found.'), $pref, $suff, $label);
}

function imagepicker_admin_view($img_id, $account = FALSE) {
  if ($account) {
    $user = $account;
    $userdir = array('name' => $user->name , 'uid' => $user->uid);
  }
  else {
    global $user;
    $userdir = FALSE;
  }

  $img = _imagepicker_get_img($img_id, FALSE, $user);
  if ($img) {
    $imgpath = imagepicker_get_image_path($img, 'full', $userdir);
    $info = image_get_info(imagepicker_get_path(FALSE, $userdir) . $img['img_name']);
    $content = theme('imagepicker_view', $img, $imgpath, $info) .
    drupal_get_form('imagepicker_admin_image_form', $img_id, $user) .
    '<br />'.
    (_imagepicker_has_groups($user) ? drupal_get_form('imagepicker_group_images_form', $img['img_id'], $user) : '');
  }
  else {
    drupal_set_message(t('Image not found.'), 'error');
    $content = '';
  }
  return $content;
}

function imagepicker_admin_image_form($form_state, $img_id, $account = FALSE) {
  if ($account) {
    $form['account'] = array(
      '#type' => 'hidden',
      '#value' => $account->uid,
    );
  }
  $form['img_id'] = array(
    '#type' => 'hidden',
    '#value' => $img_id,
  );
  $form['edit'] = array(
    '#type' => 'submit',
    '#value' => t('Edit image'),
  );
  $form['delete'] = array(
    '#type' => 'submit',
    '#value' => t('Delete image'),
  );
  return $form;
}

function imagepicker_admin_image_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Delete image')) {
    $user = user_load(array('uid' => $form_state['values']['account']));
    imagepicker_image_delete($form_state['values']['img_id'], $user, 'admin');
  }
  elseif ($form_state['values']['op'] == t('Edit image')) {
    drupal_goto('admin/settings/imagepicker/images/user/'. $form_state['values']['account'] .'/edit/'. $form_state['values']['img_id'] .'');
  }
}

function imagepicker_user_search_form($form_state, $dir) {

  $form['imagepicker_currentuser'] = array(
    '#type' => 'textfield',
    '#title' => t('Users'),
    '#description' => '',
    '#autocomplete_path' => 'admin/settings/imagepicker/'. $dir .'/autocomplete',
    '#default_value' => '',
    '#maxlength' => 30,
    '#size' => 25,
    '#prefix' => '<div id="imgp_users_form" class="container-inline">',
  );
  $form['dir'] = array(
    '#type' => 'hidden',
    '#value' => $dir,
  );
  $form['imagepicker_stats_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Go',
    '#suffix' => '</div>',
  );
  return $form;
}

function imagepicker_user_search_form_submit($form, &$form_state) {
  if ($form_state['values']['imagepicker_currentuser']) {

    $result = db_query("SELECT u.uid FROM {users} u, {imagepicker} i WHERE u.name = '%s' AND u.uid=i.uid", $form_state['values']['imagepicker_currentuser']);
    $row = db_fetch_array($result);
    if ($row['uid'] ) {
      variable_set('imagepicker_currentuser', $row['uid']);
      drupal_goto('admin/settings/imagepicker/'. $form_state['values']['dir'] .'/user/'. $row['uid']);
    }
    else {
      variable_del('imagepicker_currentuser');
    }
  }
  else {
    variable_del('imagepicker_currentuser');
  }
}

function imagepicker_fstype() {
  $dir = imagepicker_get_files_directory();
  $alphadir = 'a';
  if ( is_dir($dir . DIRECTORY_SEPARATOR . $alphadir)) {
    variable_set('imagepicker_fstype', 'alpha');
  }
  else {
    variable_set('imagepicker_fstype', 'numeric');
  }
}

/**
 * files and conversion
 */
function imagepicker_convert_form($form_state) {

  $dir = imagepicker_get_files_directory();
  imagepicker_fstype();
  $fstype = variable_get('imagepicker_fstype', '');

  if ( empty($form_state['storage']['imagepicker_files_submit'])) {
    $step = 1;
    variable_del('imagepicker_files_validate_failed');
    variable_del('imagepicker_convert_to_numeric_failed');
    variable_del('imagepicker_convert_to_alpha_failed');
  }
  else {
    $validate_failed = variable_get('imagepicker_files_validate_failed', FALSE);
    $numeric_failed = variable_get('imagepicker_convert_to_numeric_failed', FALSE);
    $alpha_failed = variable_get('imagepicker_convert_to_alpha_failed', FALSE);
    if ($validate_failed || $numeric_failed || $alpha_failed) {
      $step = 1;

    }
    else {
      $step = $form_state['values']['step']+1;
      if ($step > 2) {
        $step = 1;
      }
    }
  }

  $form['imagepicker_files'] = array(
    '#type' => 'fieldset',
    '#title' => t('Imagepicker files validation'),
    '#description' => t('The current filesystem type is %fstype and the current path is %dir', array('%fstype' => $fstype, '%dir' => $dir )),
    '#collapsible' => TRUE,
    '#collapsed' => ($fstype == 'numeric' ? TRUE : FALSE),
    '#weight' => ($fstype == 'numeric' ? 10 : -10),
  );
  $form['imagepicker_files']['step'] = array(
    '#type' => 'hidden',
    '#value' => $step,
  );

  switch ($step) {
    case 1:
      $form['imagepicker_files']['imagepicker_files_validate_label'] = array(
        '#prefix' => '<p>',
        '#value' => t('Validate'),
        '#suffix' => '</p>',
      );
      $form['imagepicker_files']['imagepicker_files_validate_label1'] = array(
        '#prefix' => '<div class="description">',
        '#value' => t('check the files directory for anomalies'),
        '#suffix' => '</div>',
      );
      $form['imagepicker_files']['imagepicker_files_validate'] = array(
        '#type' => 'hidden',
        '#value' => 1,
      );
      break;

    case 2:
      $form['imagepicker_files']['imagepicker_files_validate_label'] = array(
        '#prefix' => '<p>',
        '#value' => t('Validation done'),
        '#suffix' => '</p>',
      );
      $form['imagepicker_files']['imagepicker_files_validate'] = array(
        '#type' => 'hidden',
        '#value' => 1,
      );
      if ($fstype == 'alpha') {
        $form['imagepicker_files']['imagepicker_files_convert_to_numeric_label'] = array(
          '#prefix' => '<p>',
          '#value' => t('Next: Convert to numeric. Recommended'),
          '#suffix' => '</p>',
        );
        $form['imagepicker_files']['imagepicker_files_convert_to_numeric'] = array(
          '#type' => 'hidden',
          '#value' => 1,
        );
      }
      else {
        $form['imagepicker_files']['imagepicker_files_convert_to_alpha'] = array(
          '#type' => 'checkbox',
          '#title' => t('Convert to alpha'),
          '#description' => t('NOT recommended'),
          '#return_value' => 1,
          '#default_value' => 0,
        );
      }
      break;
  }

  $form['imagepicker_files']['imagepicker_submit_label'] = array(
    '#prefix' => '<p>',
    '#value' => '&nbsp;',
    '#suffix' => '</p>',
  );
  $form['imagepicker_files']['imagepicker_files_submit'] = array(
    '#type' => 'submit',
    '#value' => ($step==1 ? t('Validate') : t('Convert')),
  );
  return $form;
}

function imagepicker_convert_form_submit($form, &$form_state) {
  $dir = imagepicker_get_files_directory();
  $form_state['storage']['imagepicker_files_submit'] = TRUE;
  if ($form_state['values']['step'] == 1 && $form_state['values']['imagepicker_files_validate'] == 1) {
    // run validator
    $result = imagepicker_files_validate($dir);
    if (! $result) {
      // failed
      variable_set('imagepicker_files_validate_failed', TRUE);
    }
  }
  elseif ($form_state['values']['step'] == 2 && $form_state['values']['imagepicker_files_convert_to_numeric'] == 1) {
    // convert to numeric
    $result = imagepicker_convert_to_numeric($dir);
    if (! $result) {
      // failed
      variable_set('imagepicker_convert_to_numeric_failed', TRUE);
      $form_state['values']['step'] = 1;
    }
  }
  elseif ($form_state['values']['step'] == 2 && $form_state['values']['imagepicker_files_convert_to_alpha'] == 1) {
    // convert to alpha
    $result = imagepicker_convert_to_alpha($dir);
    if (! $result) {
      // failed
      variable_set('imagepicker_convert_to_alpha_failed', TRUE);
      $form_state['values']['step'] = 1;
    }
  }
}

function imagepicker_convert_to_numeric($dir) {

  $result = TRUE;
  $errlist = array();

  // work over the oldstyle dirs
  $dirlist = range('a', 'z');
  array_push($dirlist, 'others');
  foreach ($dirlist AS $alphadir) {
    $newdir = $dir . DIRECTORY_SEPARATOR . $alphadir;
    if (is_dir($newdir)) {
      // old style dir exists, now look to see if it has any users
      $newdirlist = file_scan_directory($newdir, ".*", array('.', '..', 'CVS'), 0, FALSE);
      if (count($newdirlist)) {
        foreach ($newdirlist AS $k => $v) {
          if ($newdirlist[$k]->basename) {
            $username = $newdirlist[$k]->basename;
            // get the user uid
            $uid = FALSE;
            $result = db_query("SELECT uid FROM {users} WHERE name='%s' AND status=1", array($username));
            $row = db_fetch_array($result);
            if ($row['uid']) {
              $uid = $row['uid'];
            }
            if ($uid) {
              $newpath = $dir . DIRECTORY_SEPARATOR . $uid;
              $thumbstarget = $newpath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR;
              $browsertarget = $newpath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR;

              // create dirs
              if (! file_check_directory($newpath, TRUE)) {
                $errlist[$newpath] = "Could not create dir";
              }
              if (! file_check_directory($thumbstarget, TRUE)) {
                $errlist[$thumbstarget] = "Could not create thumbs dir";
              }
              if (! file_check_directory($browsertarget, TRUE)) {
                $errlist[$browsertarget] = "Could not create browser dir";
              }

              if (! count($errlist) ) {
                // clear
                drupal_get_messages();
                $oldpath = $newdirlist[$k]->filename;
                // do here: move oldpath/* to newpath and delete oldpath
                // needs to be done per-file and dir
                $oldfilelist = file_scan_directory($oldpath, ".*", array('.', '..', 'CVS'), 0, FALSE);
                if (count($oldfilelist)) {
                  foreach ($oldfilelist as $k1 => $v1) {
                    if (! is_dir($oldfilelist[$k1]->filename) && file_exists($oldfilelist[$k1]->filename)) {
                      $file = $oldfilelist[$k1]->filename;
                      $thumbsfile = $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR . DIRECTORY_SEPARATOR . $oldfilelist[$k1]->basename;
                      $browserfile =  $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR . DIRECTORY_SEPARATOR . $oldfilelist[$k1]->basename;
                      # file_move(&$source, $dest = 0, $replace = FILE_EXISTS_RENAME)  FILE_EXISTS_ERROR
                      if (! file_move($file,  $newpath, FILE_EXISTS_ERROR)) {
                        $errlist[$file] = "Could not move to $newpath";
                      }
                      if (! file_move($thumbsfile, $thumbstarget, FILE_EXISTS_ERROR)) {
                        $errlist[$thumbsfile] = "Could not move to $thumbstarget";
                      }
                      if (! file_move($browserfile, $browsertarget, FILE_EXISTS_ERROR)) {
                        $errlist[$browserfile] = "Could not move to $browsertarget";
                      }
                    }
                  }
                }

                if (! count($errlist)) {
                  $delpath[] = $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR;
                  $delpath[] = $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR;
                  $delpath[] = $oldpath;
                  $delpath[] = $newdir;
                }
              }
            }
            else {
              $errlist[$k] = "NO uid for user $username";
            }
          }
        }
      }
      else {
        // newdir exists but is empty so delete it
        $delpath[] = $newdir;
      }
    }
  }

  if (! count($errlist)) {
    // tidy up
    if (count($delpath)) {
      foreach ($delpath AS $p) {
        if (is_dir($p)) {
          if (! rmdir($p)) {
            $errlist[$p] = 'deletion failed';
          }
        }
        else {
          $errlist[$p] = 'not a dir';
        }
      }
    }
  }

  if (count($errlist)) {
    foreach ($errlist as $key => $val) {
      drupal_set_message("ERRORS: $key: $val ", 'error');
    }
    $result = FALSE;
  }
  return $result;
}

function imagepicker_convert_to_alpha($dir) {

  $result = TRUE;
  $errlist = array();
  $uiddirlist = file_scan_directory($dir, ".*", array('.', '..', 'CVS'), 0, FALSE);

  if (! imagepicker_make_alphadirs($dir) ) {
    drupal_set_message( t('Directories could not be created'), 'error');
    return FALSE;
  }

  if (count($uiddirlist)) {
    foreach ($uiddirlist AS $uiddir => $uid) {
      if ($uiddirlist[$uiddir]->basename) {
        $useruid = $uiddirlist[$uiddir]->basename;

        // get the user name
        $result = db_query("SELECT uid, name FROM {users} WHERE uid=%d AND status=1", array($useruid));
        $row = db_fetch_array($result);
        if ($row['name']) {
          $username = $row['name'];
        }
        if ($username) {
          $firstletter = drupal_strtolower(drupal_substr($username, 0, 1));
          $firstletter = preg_match('/^[a-z]$/', $firstletter) ? $firstletter : 'others';
          $newpath = $dir . DIRECTORY_SEPARATOR . $firstletter . DIRECTORY_SEPARATOR . $username;
          $thumbstarget = $newpath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR;
          $browsertarget = $newpath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR;
          // create dirs
          if (! file_check_directory($newpath, TRUE)) {
            $errlist[$newpath] = "Could not create dir";
          }
          if (! file_check_directory($thumbstarget, TRUE)) {
            $errlist[$thumbstarget] = "Could not create thumbs dir";
          }
          if (! file_check_directory($browsertarget, TRUE)) {
            $errlist[$browsertarget] = "Could not create browser dir";
          }
          if (! count($errlist) ) {
            // clear
            drupal_get_messages();
            $oldpath = $uiddir;
            $newfilelist = file_scan_directory($oldpath, ".*", array('.', '..', 'CVS'), 0, FALSE);
            if ( count($newfilelist) ) {
              foreach ($newfilelist as $k => $v) {
                if (! is_dir($k) ) {
                  $file = $newfilelist[$k]->filename;
                  $thumbsfile = $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR . DIRECTORY_SEPARATOR . $newfilelist[$k]->basename;
                  $browserfile =  $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR . DIRECTORY_SEPARATOR . $newfilelist[$k]->basename;
                  if (! file_move($file,  $newpath, FILE_EXISTS_ERROR)) {
                    $errlist[$file] = "Could not move to $newpath";
                  }
                  if (! file_move($thumbsfile, $thumbstarget, FILE_EXISTS_ERROR)) {
                    $errlist[$thumbsfile] = "Could not move to $thumbstarget";
                  }
                  if (! file_move($browserfile, $browsertarget, FILE_EXISTS_ERROR)) {
                    $errlist[$browserfile] = "Could not move to $browsertarget";
                  }
                }
              }
            }
            if (! count($errlist) ) {
              $delpath[] = $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR;
              $delpath[] = $oldpath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR;
              $delpath[] = $oldpath;
            }
          }
        }
        else {
          $errlist[$k] = "NO uid for user $u";
        }
      }
    }
  }

  if (! count($errlist) ) {
    // tidy up
    if ( count($delpath) ) {
      foreach ($delpath AS $p) {
        if (is_dir($p)) {
          if (! rmdir($p)) {
            $errlist[$p] = 'deletion failed';
          }
        }
        else {
          $errlist[$p] = 'not a dir';
        }
      }
    }
  }

  if (count($errlist)) {
    foreach ( $errlist as $key => $val ) {
      drupal_set_message("ERRORS: $key: $val ", 'error');
    }
    $result = FALSE;
  }
  return $result;

}

function imagepicker_make_alphadirs($dir) {

  if (file_check_directory($dir, TRUE)) {
    $dirlist = range('a', 'z');
    array_push($dirlist, 'others');

    foreach ($dirlist AS $v) {
      $newdir = $dir . DIRECTORY_SEPARATOR . $v;
      if (! file_check_directory($newdir, TRUE)) {
        drupal_set_message(t('Directory creation for the Imagepicker module was unsuccessful.'), 'error');
      }
    }
    $errors = drupal_get_messages('error');
    if (! count($errors)) {
      // clear
      drupal_get_messages();
      drupal_set_message(t('Directory creation for the Imagepicker module was successful.'));
      return $dir;
    }
    else {
      foreach ($errors['error'] as $error) {
        drupal_set_message($error, 'error');
      }
    }
  }
  else {
    drupal_set_message(t('Directory creation for the Imagepicker module was unsuccessful.'), 'error');
  }
  return FALSE;
}

function imagepicker_files_validate($dir) {
  $fstype = variable_get('imagepicker_fstype', '');
  $result = TRUE;
  $errlist = array();
  $uidlist = array();
  if ($fstype == 'numeric' ) {
    $baddirlist = array();
    $numericdirlist = file_scan_directory($dir, ".*", array('.', '..', 'CVS'), 0, FALSE);
    foreach ( $numericdirlist AS $k => $v ) {
      if ( is_numeric($numericdirlist[$k]->basename) ) {
        $numericdir = $numericdirlist[$k]->basename;
        $result = db_query("SELECT uid FROM {users} WHERE uid=%d AND status=1", array($numericdir));
        $row = db_fetch_array($result);
        if ($row['uid']) {
          // build list of valid paths to images, use uid as key
          $uidlist[$row['uid']] = $numericdirlist[$k]->filename;
        }
        else {
          array_push($baduseridlist, $numericdir);
        }
      }
    }
    if ( count($baduseridlist) ) {
      $errlist['user ids do not exist'] = implode(',', $baduseridlist);
    }
  }
  elseif ($fstype == 'alpha' ) {
    // check that a-z and other exist, log errors
    $dirlist = range('a', 'z');
    array_push($dirlist, 'others');
    $baddirlist = array();
    $gooddirlist = array();
    foreach ($dirlist AS $alphadir) {
      if (! is_dir($dir . DIRECTORY_SEPARATOR . $alphadir)) {
        array_push($baddirlist, $alphadir);
      }
      else {
        array_push($gooddirlist, $alphadir);
      }
    }
    if (count($baddirlist)) {
      $errlist['missing dirs'] = implode(',', $baddirlist);
    }
    // get the first level dirs actually present and check for unknowns
    $baddirlist = array();
    $alphadirlist = file_scan_directory($dir, ".*", array('.', '..', 'CVS'), 0, FALSE);
    foreach ($alphadirlist AS $k => $v) {
      $alphadir = $alphadirlist[$k]->basename;
      if (! in_array($alphadir , $dirlist)) {
        array_push($baddirlist, $alphadir);
      }
    }
    if (count($baddirlist)) {
      $errlist['Anomalous directories'] = implode(',', $baddirlist);
    }
    // search for usernames and validate them
    $userdirlist = array();
    $userdirlistpath = array();
    foreach ($gooddirlist AS $alphadir) {
      $gooddir = $dir . DIRECTORY_SEPARATOR . $alphadir;
      $userdirlisttmp = file_scan_directory($gooddir, ".*", array('.', '..', 'CVS'), 0, FALSE);
      if (count($userdirlisttmp)) {
        foreach ($userdirlisttmp AS $k => $v) {
          array_push($userdirlist, $userdirlisttmp[$k]->basename);
          array_push($userdirlistpath, $userdirlisttmp[$k]->filename);
        }
      }
    }
    // we have usernames, now look them up
    $badusernamelist = array();
    foreach ( $userdirlist AS $k => $username ) {
      $result = db_query("SELECT uid FROM {users} WHERE name='%s' AND status=1", array($username));
      $row = db_fetch_array($result);
      if ($row['uid']) {
        // build list of valid paths to images, use uid as key
        $uidlist[$row['uid']] = $userdirlistpath[$k];
      }
      else {
        array_push($badusernamelist, $username);
      }
    }
    if ( count($badusernamelist) ) {
      $errlist['bad usernames'] = implode(',', $badusernamelist);
    }
  }

  // check valid uids against the imagepicker table, get the file list for each
  $badfiledirs = array();
  $badfilelist = array();
  $badthumbsfilelist = array();
  $badbrowserfilelist = array();
  $badfilelist2 = array();
  $badwritefiles = array();

  $files = array();
  $thumbsfiles = array();
  $browserfiles = array();
  foreach ($uidlist AS $uid => $filepath) {
    $imgnames = array();
    $result = db_query("SELECT img_name FROM {imagepicker} WHERE uid=%d", array($uid));
    while ($row = db_fetch_array($result)) {
      array_push($imgnames, $row['img_name']);
    }
    $filestmp = file_scan_directory($filepath, ".*", array('.', '..', 'CVS', IMAGEPICKER_THUMBS_DIR, IMAGEPICKER_BROWSER_DIR), 0, FALSE);
    if (count($filestmp)) {
      // compare files with db
      foreach ($filestmp AS $k => $v ) {
        $file = $filestmp[$k]->basename;
        array_push($files, $file);
        if (! is_writable($filestmp[$k]->filename) ) {
          array_push($badwritefiles, $filestmp[$k]->filename);
        }
        $thumbsfile = $filepath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR . DIRECTORY_SEPARATOR . $file;
        array_push($thumbsfiles, $thumbsfile);
        $browserfile = $filepath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR . DIRECTORY_SEPARATOR . $file;
        array_push($browserfiles, $browserfile);
        // test for presence in db
        if (! in_array($file , $imgnames) ) {
          array_push($badfilelist, $file);
        }
        if (! file_exists($thumbsfile)) {
          array_push($badthumbsfilelist, $thumbsfile);
        }
        elseif (! is_writable($thumbsfile)) {
          array_push($badwritefiles, $thumbsfile);
        }
        if (! file_exists($browserfile)) {
          array_push($badbrowserfilelist, $browserfile);
        }
        elseif (! is_writable($browserfile)) {
          array_push($badwritefiles, $browserfile);
        }
      }
    }
    else {
      array_push($badfiledirs, $uid);
    }
    // compare db to files
    if (count($imgnames) ) {
      foreach ($imgnames AS $imgname ) {
        if (! in_array($imgname, $files) ) {
          array_push($badfilelist2, $filepath . DIRECTORY_SEPARATOR . $imgname);
          $thumbsfile = $filepath . DIRECTORY_SEPARATOR . IMAGEPICKER_THUMBS_DIR . DIRECTORY_SEPARATOR . $imgname;
          if (! file_exists($thumbsfile)) {
            array_push($badthumbsfilelist, $thumbsfile);
          }
          $browserfile = $filepath . DIRECTORY_SEPARATOR . IMAGEPICKER_BROWSER_DIR . DIRECTORY_SEPARATOR . $imgname;
          if (! file_exists($browserfile)) {
            array_push($badbrowserfilelist, $browserfile);
          }
        }
      }
    }
  }

  if (count($badfilelist)) {
    $errlist[t('file not found in database')] = implode(',', $badfilelist);
  }
  if (count($badfilelist2)) {
    $errlist[t('database image name not found in files')] = implode(',', $badfilelist2);
  }
  if (count($badthumbsfilelist)) {
    $errlist[t('thumbs file does not exist')] = implode(',', $badthumbsfilelist);
  }
  if (count($badbrowserfilelist)) {
    $errlist[t('browser file does not exist')] = implode(',', $badbrowserfilelist);
  }
  if (count($badfiledirs)) {
    $errlist[t('uid has no files')] = implode(',', $badfiledirs);
  }
  if ( count($badwritefiles)) {
    $errlist[t('file not writable')] = implode(',', $badwritefiles);
  }

  if (count($errlist) ) {
    foreach ($errlist AS $k => $v) {
      drupal_set_message("$k: $v", 'error');
    }
    drupal_set_message(t('Validation of directory structure failed, please correct the above errors before continuing.'), 'error');
    $result = FALSE;
  }
  else {
    drupal_set_message(t('Directory structure validated, %uidlist users found', array('%uidlist' => count($uidlist))));
  }
  return $result;
}

function imagepicker_admin_groups() {
  drupal_add_css(drupal_get_path('module', 'imagepicker') .'/imagepicker.css');
  include_once('imagepicker.group.inc');
  if (! arg(4) ) {
    variable_del('imagepicker_currentuser');
    variable_set('imagepicker_currentgroup', 0);
    $action = 'users';
  }
  $uid = variable_get('imagepicker_currentuser', '');
  if ( arg(4) == 'user' && is_numeric(arg(5))) {
    $uid = arg(5);
  }
  elseif (arg(4) == 'stats' || arg(4) == 'users') {
    $action = arg(4);
  }
  if ( (arg(6) == 'browse' || arg(6) == 'edit' || arg(6) == 'delete') && is_numeric(arg(7)) ) {
    $gidaction = arg(6);
    $gid = arg(7);
    if (arg(6) == 'browse') {
      variable_set('imagepicker_currentgroup', $gid);
      drupal_goto('admin/settings/imagepicker/images/user/'. $uid .'/browse');
    }
  }
  $name = "";
  if ($uid) {
    $user = user_load(array('uid' => $uid));
    $result = db_query("SELECT u.name FROM {users} u, {imagepicker} i WHERE u.uid = %d AND u.uid=i.uid", $uid);
    $row = db_fetch_array($result);
    $name = $row['name'];
  }
  $content .= drupal_get_form('imagepicker_group_search_form');
  $content .= '<p>'. t('Managing groups for ') . ($name ? l($name, "user/$uid") : 'All') .'</p>';
  if ($uid) {
    // show groups for $uid
    if ($gid) {
      if ($gidaction == 'edit') {
        $record = imagepicker_get_user_group($gid);
        $content .= drupal_get_form('imagepicker_groups_form', $record);
      }
      elseif ($gidaction == 'delete') {
        $content .= drupal_get_form('imagepicker_group_delete_form', $gid);
      }
    }
    elseif (arg(6) == 'stats') {
      $content .= imagepicker_group_stats($user, t('Statistics for %name', array('%name' => $name)));
    }
    else {
      $content .= imagepicker_groups_list('admin', $user, t('Groups for %name', array('%name' => $name)));
      $content .= drupal_get_form('imagepicker_groups_form', 0, $user);
    }
  }
  else {
    if ($action == 'stats') {
      $content .= imagepicker_group_stats(FALSE, t('Statistics'));
    }
    else {
      $content .= imagepicker_admin_groups_list(t('Groups'));
    }
  }
  return $content;
}

function imagepicker_admin_images() {
  drupal_add_css(drupal_get_path('module', 'imagepicker') .'/imagepicker.css');

  if (! arg(4) ) {
    variable_del('imagepicker_currentuser');
    variable_set('imagepicker_currentgroup', 0);
    $action = 'users';
  }
  $uid = variable_get('imagepicker_currentuser', '');
  if ( arg(4) == 'user' && is_numeric(arg(5))) {
    $uid = arg(5);
  }
  elseif (arg(4) == 'browse_public' || arg(4) == 'stats' || arg(4) == 'users' || arg(4) == 'list_public') {
    $action = arg(4);
    variable_del('imagepicker_currentuser');
    variable_set('imagepicker_currentgroup', 0);
    $uid = variable_get('imagepicker_currentuser', '');
  }
  if ( arg(6) == 'browse_public'
    || arg(6) == 'browse'
    || arg(6) == 'edit'
    || arg(6) == 'delete'
    || arg(6) == 'browseadmin'
    || arg(6) == 'upload'
    || arg(6) == 'stats'
    || arg(6) == 'groups'
    || arg(6) == 'quota') {
    $imgaction = arg(6);
  }
  if ( is_numeric(arg(7))) {
    $img_id = arg(7);
  }
  if (arg(6) == 'upload' ) {
    include_once('imagepicker.upload.inc');
    if (variable_get('imagepicker_upload_progress_enabled', 1)) {
      $content .= imagepicker_upload_progress_get_script(variable_get('imagepicker_upload_progress_delay', 0));
    }
  }

  $name = "";
  if ($uid) {
    $user = user_load(array('uid' => $uid));
    $name = $user->name;
  }
  $content .= drupal_get_form('imagepicker_user_search_form', 'images');
  $content .= '<p>'. t('Managing images for ') . ($name ? l($name, "user/$uid") : 'All') .'</p>';

  if ($uid) {
    if ($img_id) {
      if ($imgaction == 'edit' ) {
        include_once('imagepicker.edit.inc');
        $img =  _imagepicker_get_img($img_id, FALSE, $user);
        $imgsrc = imagepicker_get_image_path($img, 'browser', array('uid' => $user->uid , 'name' => $user->name) );
        $content .= theme('imagepicker_image_edit_header', $img, $imgsrc);
        $content .= drupal_get_form('imagepicker_edit_form', $img, 'admin', $user);
      }
      elseif ($imgaction == 'delete') {
        imagepicker_image_delete($img_id, $user, 'admin');
      }
      else {
        $content .= imagepicker_admin_view($img_id, $user);
      }
    }
    elseif ($imgaction == 'upload') {
      $content .= imagepicker_quota_ok('admin', $user, t('Upload file for %name', array('%name' => $name)));
    }
    elseif ($imgaction == 'stats') {
      $content .= imagepicker_group_stats($user, t('Statistics for %name', array('%name' => $name)));
    }
    elseif ($imgaction == 'browseadmin') {
      $content .= _imagepicker_browse_admin('admin', $user, FALSE, 1, t('List images for %name', array('%name' => $name)));
    }
    elseif ($imgaction == 'quota') {
      $usedbytes = imagepicker_get_all_bytes($user)+1;
      $usedbytesprint = _imagepicker_bkmg($usedbytes);
      $quota = 0;
      if ( variable_get('imagepicker_quota_enable', 1) ) {
        $quota = (isset($user->imagepicker_quota) ? $user->imagepicker_quota : variable_get('imagepicker_quota_default', 1));
      }
      $label = t('Quota for %name', array('%name' => $name));
      $imgtot = _imagepicker_user_has_img($user);
      $pl = format_plural($imgtot, '1 image', '@count images');
      if ($quota) {
        $pused = round($usedbytes / ($quota*1000000) * 100, 2);
        $message = t('User %name has used %pused percent or %usedbytesprint of quota and has %pl.', array('%name' => $name, '%pused' => $pused, '%usedbytesprint' => $usedbytesprint, '%pl' => $pl));
      }
      else {
        $message = t('User %name has used %usedbytesprint and has %pl.', array('%name' => $name, '%usedbytesprint' => $usedbytesprint, '%pl' => $pl));
      }
      $form = drupal_get_form('imagepicker_quota_form', $user);
      $content .= theme('imagepicker_quota', $form, $message, $label);
    }
    elseif ($imgaction == 'groups') {
      drupal_goto('admin/settings/imagepicker/groups/user/'. $uid);
    }
    else {
      $content .= _imagepicker_browse('admin', $user, t('Browse images for %name', array('%name' => $name)));
    }
  }
  else {
    if ($action == 'stats') {
      $content .= imagepicker_group_stats(FALSE, t('Statistics'));
    }
    elseif ($action == 'browse_public') {
      $content .= _imagepicker_browse_public('admin', variable_get('imagepicker_browse_public', 0), t('Browse All Images'));
    }
    elseif ($action == 'list_public') {
      $content .= _imagepicker_browse_admin('admin', FALSE, TRUE, variable_get('imagepicker_browse_public', 0), t('List All Images'));
    }
    else {
      $content .= imagepicker_admin_users_list('images', t('Users'));
    }
  }
  return $content;
}

function imagepicker_group_search_form($form_state, $dir = 'groups') {

  $form['imagepicker_currentgroup'] = array(
    '#type' => 'textfield',
    '#title' => t('Groups'),
    '#description' => '',
    '#autocomplete_path' => 'admin/settings/imagepicker/groups/autocomplete',
    '#default_value' => '',
    '#maxlength' => 30,
    '#size' => 25,
    '#prefix' => '<div id="imgp_groups_form" class="container-inline">',
  );
  $form['dir'] = array(
    '#type' => 'hidden',
    '#value' => $dir,
  );
  $form['imagepicker_group_search_submit'] = array(
    '#type' => 'submit',
    '#value' => 'Go',
    '#suffix' => '</div>',
  );
  return $form;
}

function imagepicker_group_search_form_submit($form, &$form_state) {
  if ($form_state['values']['imagepicker_currentgroup']) {
    $result = db_query("SELECT gid, uid FROM {imagepicker_user_groups} WHERE group_name = '%s' ", $form_state['values']['imagepicker_currentgroup']);
    $row = db_fetch_array($result);
    if ($row['gid'] ) {
      variable_set('imagepicker_currentgroup', $row['gid']);
      drupal_goto('admin/settings/imagepicker/'. $form_state['values']['dir'] .'/user/'. $row['uid'] .'/edit/'. $row['gid']);
    }
    else {
      variable_set('imagepicker_currentgroup', 0);
    }
  }
  else {
    variable_set('imagepicker_currentgroup', 0);
  }
}

/**
 * quotas
 *
 */
function imagepicker_quota_form(&$form_state, $account=FALSE) {
  $list = imagepicker_get_quota_list();
  $default = variable_get('imagepicker_quota_default', 0);
  $quota = $default;
  $title = t('Set default quota');
  if ($account) {
    $title = t('Set user quota');
    $quota = $account->imagepicker_quota;
    if (! $quota) {
      $quota = $default;
    }
  }
  $form['imagepicker_quota'] = array(
    '#type' => 'select',
    '#default_value' => $quota,
    '#options' => $list,
    '#title' => $title,
    '#prefix' => '<div id="imgp_quota_form" class="container-inline">',
  );
  $form['account'] = array(
    '#type' => 'hidden',
    '#value' => ($account ? $account->uid : 0),
  );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Go'),
    '#suffix' => '</div>'
  );
  return $form;
}

function imagepicker_quota_form_submit($form, &$form_state) {

    if ( $form_state['values']['account'] > 0 ) {
      $user = user_load(array('uid' => $form_state['values']['account']));
      user_save($user, array('imagepicker_quota' => $form_state['values']['imagepicker_quota']));
    }
    else {
      variable_set('imagepicker_quota_default', $form_state['values']['imagepicker_quota']);
    }
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for existing users
 */
function imagepicker_user_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_query_range("SELECT u.name FROM {users} u, {imagepicker} i WHERE LOWER(u.name) LIKE LOWER('%s%%') AND u.uid=i.uid", $string, 0, 10);
    while ($user = db_fetch_object($result)) {
      $matches[$user->name] = check_plain($user->name);
    }
  }
  print drupal_to_js($matches);
  exit();
}

/**
 * Retrieve a pipe delimited string of autocomplete suggestions for existing groups
 */
function imagepicker_group_search_autocomplete($string = '') {
  $matches = array();
  if ($string) {
    $result = db_query_range("SELECT group_name FROM {imagepicker_user_groups} WHERE LOWER(group_name) LIKE LOWER('%s%%')", $string, 0, 10);
    while ($group = db_fetch_object($result)) {
      $matches[$group->group_name] = check_plain($group->group_name);
    }
  }
  print drupal_to_js($matches);
  exit();
}
