<?php
// $Id$
// $Name$

/**
 * Menu callback; fetches the image edit form for imagepicker
 */
function imagepicker_image_edit() {
  global $user, $base_url;

  $result = db_query_range("SELECT * FROM {imagepicker} WHERE img_id = '%d'", arg(2), 0, 1);
  $img = db_fetch_array($result);
  if ($img) {
    if ($img['uid'] != $user->uid) {
      drupal_set_message(t('This image does not belong to you.'), 'error');
      watchdog('imagepicker', 'User uid %d attempted to edit image belonging to user uid %d', array($user->uid, $img['uid']), WATCHDOG_WARNING);
      $content = '';
    }
    else {
      $imgsrc = imagepicker_get_image_path($img, 'browser');
      $content = '<div id="imgp_img_holder"><img src="'. $imgsrc .'" alt="'. check_plain($img['img_title']) .'" /></div>';
      $content .= drupal_get_form('imagepicker_edit_form', $img);
    }
  }
  else {
    drupal_set_message(t('Image  not found.'), 'error');
    $content = '';
  }

  theme('imagepicker', $content);
}

function imagepicker_edit_form(&$form_state, $img) {
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
  $form['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Submit'),
    '#prefix' => '<div id="imgp_controls">'
    );
  $form['cancel'] = array(
    '#type' => 'submit',
    '#value' => t('Cancel'),
    '#suffix' => '</div>'
    );
  return $form;
}

/**
 * Submit form
 */
function imagepicker_edit_form_submit($form, &$form_state) {
  if ($form_state['values']['op'] == t('Submit')) {
    global $user;
    $img_id = arg(2);
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
          drupal_goto('imagepicker/browse/'. $img_id);
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
  }
  drupal_goto('imagepicker/browse');
}
