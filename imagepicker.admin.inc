<?php
// $Id$
// $Name$

/**
 * Menu callback; presents the settings form for imagepicker
 */
function imagepicker_admin_settings() {

  // default settings
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
  $form['imagepicker_defaults']['imagepicker_default_browser_thumbnail_size'] = array(
    '#type' => 'textfield',
    '#title' => t('Default Browser Thumbnail Size'),
    '#size' => 10,
    '#required' => TRUE,
    '#default_value' => variable_get('imagepicker_default_browser_thumbnail_size', 100),
    '#description' => t('Configure the default browser thumbnail size'),
  );
  $node_types = node_get_types('names');
  $form['imagepicker_defaults']['imagepicker_node_types_enabled'] = array(
    '#type' => 'checkboxes',
    '#title' => t('Node Types'),
    '#description' => t('Set the node types you want to enable Imagepicker for.'),
    '#default_value' => variable_get('imagepicker_node_types_enabled', array_keys($node_types)),
    '#options' => $node_types,
  );
  if (module_exists('comment')) {
    $form['imagepicker_defaults']['imagepicker_comment_enabled'] = array(
      '#type' => 'checkbox',
      '#title' => t('Comments'),
      '#return_value' => 1,
      '#default_value' => variable_get('imagepicker_comment_enabled', 0),
      '#description' => t('Setting this option enables Imagepicker in comments.'),
    );
  }
  $form['imagepicker_defaults']['imagepicker_default_pagelink'] = array(
    '#type' => 'checkbox',
    '#title' => t('Show return link in page'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_default_pagelink', 1),
    '#description' => t('Setting this option will add a link back to the thumbnail when using the page option. Uses javascript history(back).'),
  );
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
  // advanced settings
  $form['imagepicker_advanced'] = array(
    '#type' => 'fieldset',
    '#title' => t('Advanced imagepicker settings'),
    '#description' => t('You can alter the default styling of the iframe here,<br />useful if the iframe is not fitting in with your theme.'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
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
  $form['imagepicker_advanced']['imagepicker_advanced_collapsed'] = array(
    '#type' => 'checkbox',
    '#title' => t('Fieldset state'),
    '#return_value' => 1,
    '#default_value' => variable_get('imagepicker_advanced_collapsed', 0),
    '#description' => t('Setting this option will collapse the fieldset the iframe is in by default.'),
  );

  return system_settings_form($form);
}

/**
 * Validate form
 */
function imagepicker_admin_settings_validate($form_id, $form_state) {
  if (! strlen($form_state['values']['imagepicker_default_thumbnail_size'])) {
    form_set_error('imagepicker_default_thumbnail_size', t('You must fill in the Thumbnail field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_default_thumbnail_size'])) {
    form_set_error('imagepicker_default_thumbnail_size', t('The Thumbnail field must be a number'));
  }
  if (! strlen($form_state['values']['imagepicker_default_browser_thumbnail_size'])) {
    form_set_error('imagepicker_default_browser_thumbnail_size', t('You must fill in the Browser Thumbnail field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_default_browser_thumbnail_size'])) {
    form_set_error('imagepicker_default_browser_thumbnail_size', t('The Browser Thumbnail field must be a number'));
  }
  if (strlen($form_state['values']['imagepicker_default_scale']) && ! is_numeric($form_state['values']['imagepicker_default_scale'])) {
    form_set_error('imagepicker_default_scale', t('The Scale field must be a number'));
  }
  if (! strlen($form_state['values']['imagepicker_default_fleft'])) {
    form_set_error('imagepicker_default_fleft', t('You must fill in the Left alignment field'));
  }
  if (! strlen($form_state['values']['imagepicker_default_fright'])) {
    form_set_error('imagepicker_default_fright', t('You must fill in the Right alignment field'));
  }
  if (! is_numeric($form_state['values']['imagepicker_advanced_iframe_height'])) {
    form_set_error('imagepicker_advanced_iframe_height', t('The Iframe height must be a number'));
  }
  if (! strlen($form_state['values']['imagepicker_advanced_iframe_width'])) {
    form_set_error('imagepicker_advanced_iframe_width', t('You must fill in the Iframe width'));
  }
  if (! strlen($form_state['values']['imagepicker_advanced_iframe_border'])) {
    form_set_error('imagepicker_advanced_iframe_border', t('You must fill in the Iframe border'));
  }
}
