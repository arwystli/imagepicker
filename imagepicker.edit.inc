<?php
// $Id$
// $Name$

/**
 * Menu callback; fetches the image edit form for imagepicker
 */
// iframe
function imagepicker_image_edit($img_id) {
  $content = _imagepicker_edit_img($img_id, FALSE);
  theme('imagepicker', $content);
}

// account
function imagepicker_user_image_edit($img_id) {
  $content = _imagepicker_edit_img($img_id, TRUE);
  return $content;
}

function imagepicker_edit_form(&$form_state, $img, $account=FALSE) {
  $form['title'] = array(
    '#type' => 'textfield',
    '#title' => t('Title'),
    '#description' => t('Edit title of your image'),
    '#default_value' => htmlspecialchars_decode($img['img_title']),
    '#prefix' => '<div id="imgp_edit_form">'
  );
  $form['description'] = array(
    '#type' => 'textarea',
    '#title' => t('Description'),
    '#rows' => 2,
    '#cols' => 80,
    '#description' => t('Edit description of your image'),
    '#default_value' => htmlspecialchars_decode($img['img_description']),
    '#suffix' => '</div>'
  );
  if ($account) {
    $form['account'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
  }
  $form['img_id'] = array(
      '#type' => 'hidden',
      '#value' => $img['img_id'],
    );
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#prefix' => '<div id="imgp_controls">',
    '#submit' => array('imagepicker_edit_form_process'),
  );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#suffix' => '</div>',
    '#submit' => array('imagepicker_edit_form_cancel'),
  );
  return $form;
}

/**
 * Submit form functions
 */
function imagepicker_edit_form_cancel($form, &$form_state) {
  global $user;
  if ($form_state['values']['account']) {
    $outpath = 'user/'. $user->uid .'/imagepicker/browse';
  }
  else {
    $outpath = 'imagepicker/browse/'. $img_id;
  }
  drupal_set_message(t('Cancelled.'));
  drupal_goto($outpath);
}


function imagepicker_edit_form_process($form, &$form_state) {
  global $user;
  $img_id = $form_state['values']['img_id'];
  if ($form_state['values']['account']) {
    $outpath = 'user/'. $user->uid .'/imagepicker/browse';
  }
  else {
    $outpath = 'imagepicker/browse/'. $img_id;
  }
  $result = db_query_range("SELECT uid, img_name FROM {imagepicker} WHERE img_id = '%d'", $img_id, 0, 1);
  $img = db_fetch_array($result);
  if ($img) {
    if ($img['uid'] == $user->uid) {
      $title = htmlspecialchars($form_state['values']['title']);
      $description = htmlspecialchars($form_state['values']['description']);
      $date = date('Y-m-d H:i:s');
      if (db_query("UPDATE {imagepicker} SET img_title = '%s', img_description = '%s', img_date = '%s' WHERE img_id = '%d'",
          array($title, $description, $date, $img_id))) {
        drupal_set_message(t('Image was successfully updated.'));
        drupal_goto($outpath);
      }
      else {
        drupal_set_message(t('Error while updating image.'));
      }
    }
    else {
      drupal_set_message(t('This image does not belong to you.'), 'error');
      watchdog('imagepicker', 'User uid %d attempted to edit image belonging to user uid %d', array($user->uid, $img['uid']), WATCHDOG_WARNING);
    }
  }
  else {
    drupal_set_message(t('Image not found.'), 'error');
  }

  drupal_goto($outpath);
}

/**
 * private functions
 */
function _imagepicker_edit_img($img_id, $account=FALSE) {
  $content = '';
  $img =  _imagepicker_get_img($img_id);
  if($img) {
    $imgsrc = imagepicker_get_image_path($img, 'browser');
    $content .= "<div class='imgp_help'>". t('Edit image details') ."</div>";
    $content .= '<div id="imgp_img_holder"><img src="'. $imgsrc .'" alt="'. check_plain($img['img_title']) .'" /></div>';
    $content .= drupal_get_form('imagepicker_edit_form', $img, $account);
  }
  else {
    drupal_set_message(t('Image not found in edit.'), 'error');
    $content = '';
  }

  return $content;
}

