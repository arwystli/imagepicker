<?php
// $Id$
// $Name$

/**
 * @file
 * contains the functions for group management
 */

function imagepicker_groups() {

  // from iframe
  if (! arg(2)) {
    $content = imagepicker_groups_list();
    $content .= drupal_get_form('imagepicker_groups_form');
  }
  elseif ( arg(2) == 'edit') {
    $record = imagepicker_get_user_group(arg(3));
    $content .= drupal_get_form('imagepicker_groups_form', $record);
  }
  elseif (arg(2) == 'delete') {
    $content .= drupal_get_form('imagepicker_group_delete_form', arg(3));
  }
  elseif ( is_numeric(arg(2)) && arg(2) > 0 ) {
    imagepicker_set_user_group_state(1, arg(2));
    drupal_goto('imagepicker/browse');
  }
  theme('imagepicker', $content);
}


/**
 * groups
 * from imagepicker groups in my account
 */
function imagepicker_user_groups($mode="", $gid=0) {
  // $mode is edit, delete or empty
  if (empty($mode)) {
    $content = imagepicker_groups_list('account');
    $content .= drupal_get_form('imagepicker_groups_form');
  }
  elseif ($mode == 'edit') {
    $record = imagepicker_get_user_group($gid);
    $content .= drupal_get_form('imagepicker_groups_form', $record);
  }
  elseif ($mode == 'delete') {
    $content .= drupal_get_form('imagepicker_group_delete_form', $gid);
  }
  return $content;
}

function imagepicker_groups_form(&$form_state, $record=0, $account=FALSE) {
  $form['groupsave'] = array(
    '#type' => 'fieldset',
    '#title' => ( $record ?  t('Edit group') : t('Add group')),
    '#description' => t('Give your group a brief name and optionally put any additional information in the group description box'),
    '#collapsible' => TRUE,
    '#collapsed' => FALSE,
  );
  $form['groupsave']['group_name'] = array(
    '#type' => 'textfield',
    '#title' => t('Group name'),
    '#size' => 20,
    '#default_value' => ($record ? $record->group_name : ''),
    '#description' => t(''),
    '#required' => TRUE
  );
  $form['groupsave']['group_description'] = array(
    '#type' => 'textfield',
    '#title' => t('group description'),
    '#size' => 50,
    '#maxlength' => 50,
    '#default_value' => ($record ? $record->group_description : ''),
    '#description' => t('Maximum 50 characters'),
    '#required' => FALSE
  );
  if (user_access('use public imagepicker') && variable_get('imagepicker_public_enabled', 1)) {
    $form['groupsave']['group_public'] = array(
      '#type' => 'checkbox',
      '#title' => t('Public'),
      '#return_value' => 1,
      '#default_value' => ($record->public ? $record->public : ''),
      '#description' => t('Make this group publicly available'),
    );
  }

  if ($account) {
    $form['groupsave']['account'] = array(
      '#type' => 'hidden',
      '#value' => $account->uid,
    );
  }
  if ($record->gid) {
    $form['groupsave']['gid'] = array(
      '#type' => 'hidden',
      '#value' => $record->gid,
    );
  }
  $form['groupsave']['submit'] = array(
    '#type' => 'submit',
    '#value' => t('Save group'),
  );
  return $form;
}

function imagepicker_groups_form_submit($form, &$form_state) {
  global $user;
  $record['group_name'] = $form_state['values']['group_name'];
  $record['group_description'] = $form_state['values']['group_description'];
  $record['public'] = $form_state['values']['group_public'];
  if ( $form_state['values']['account'] ) {
    $record['uid'] = $form_state['values']['account'];
  }
  else {
    $record['uid'] = $user->uid;
  }
  if ($form_state['values']['gid']) {
    $record['gid'] = $form_state['values']['gid'];
    imagepicker_update_user_group($record);
  }
  else {
    imagepicker_insert_user_group($record);
  }
}

/**
 * imagepicker_user_groups functions
 */
function imagepicker_insert_user_group($record) {
  if (db_query(
    "INSERT INTO {imagepicker_user_groups} (uid, group_name, group_description, public) VALUES (%d, '%s', '%s', %d)",
    array($record['uid'], $record['group_name'], $record['group_description'], $record['public']))) {
    drupal_set_message(t('Group was successfully inserted'));
  }
  else {
    drupal_set_message(t('Error while trying to insert your group.'));
  }
}

function imagepicker_update_user_group($record) {
  if (db_query(
    "UPDATE {imagepicker_user_groups} SET group_name='%s', group_description='%s', public=%d WHERE gid = %d",
    array($record['group_name'], $record['group_description'], $record['public'], $record['gid']))) {
    drupal_set_message(t('Group was successfully updated'));
  }
  else {
    drupal_set_message(t('Error while trying to update your group.'));
  }
}

function imagepicker_delete_user_group($gid) {
  if (db_query("DELETE FROM {imagepicker_user_groups} WHERE gid = %d", array($gid))
    && db_query("DELETE FROM {imagepicker_group_images} WHERE gid=%d", array($gid)) ) {
    drupal_set_message(t('Group was successfully deleted'));
  }
  else {
    drupal_set_message(t('Error while trying to delete group.'));
  }
}

function imagepicker_get_user_groups_by_user($uid, $obj=TRUE) {
  $result = db_query("SELECT * FROM {imagepicker_user_groups} WHERE uid = %d", array($uid));
  if ($obj) {
    return db_fetch_object($result);
  }
  return db_fetch_array($result);
}

function imagepicker_group_edit($gid) {
  $record = imagepicker_get_user_group($gid);
  $content .= drupal_get_form('imagepicker_groups_form', $record);
  return $content;
}

/**
 * group delete form
 */
function imagepicker_group_delete_form(&$form_state, $gid) {
  $record = imagepicker_get_user_group($gid);
  if ($record) {
    $form['groupname'] = array(
      '#value' => '<p>'. $record->group_name .'</p>',
    );
    $form['mode'] = array(
      '#type' => 'hidden',
      '#value' => 'reallydelete',
    );
    $form['gid'] = array(
      '#type' => 'hidden',
      '#value' => $gid,
    );
    $form['delete'] = array(
      '#type' => 'submit',
      '#value' => t('Really Delete record'),
    );
    return $form;
  }
  else {

  }
}

/**
 * Submit group delete form
 */
function imagepicker_group_delete_form_submit($form, &$form_state) {
  imagepicker_delete_user_group($form_state['values']['gid']);
}


