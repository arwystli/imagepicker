<?php
// $Id$
// $Name$

/**
 * @file
 * contains the functions for group management
 */

/**
 * groups
 * from imagepicker groups in my account
 */
function imagepicker_user_groups($mode="", $gid=0) {
  // $mode is edit, delete or empty
  if (empty($mode)) {
    $content = imagepicker_groups_list(TRUE);
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
  if ($account == TRUE) {
    $form['groupsave']['account'] = array(
      '#type' => 'hidden',
      '#value' => 1,
    );
  }
  if ($record) {
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
  $record['uid'] = $user->uid;
  if ($form_state['values']['gid']) {
    $record['gid'] = $form_state['values']['gid'];
    imagepicker_update_user_group($record);
  }
  else {
    imagepicker_insert_user_group($record);
  }
}

// imagepicker_user_groups functions
function imagepicker_insert_user_group($record) {
  if (db_query(
    "INSERT INTO {imagepicker_user_groups} (uid, group_name, group_description) VALUES (%d, '%s', '%s')",
    array($record['uid'], $record['group_name'], $record['group_description']))) {
    drupal_set_message(t('Group was successfully inserted'));
  }
  else {
    drupal_set_message(t('Error while trying to insert your group.'));
  }
}

function imagepicker_update_user_group($record) {
  if (db_query(
    "UPDATE {imagepicker_user_groups} SET group_name='%s', group_description='%s' WHERE gid = %d",
    array($record['group_name'], $record['group_description'], $record['gid']))) {
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

function imagepicker_get_user_group($gid, $obj=TRUE) {
  $result = db_query("SELECT * FROM {imagepicker_user_groups} WHERE gid = %d", array($gid));
  if ($obj) {
    return db_fetch_object($result);
  }
  return db_fetch_array($result);
}

function imagepicker_get_user_groups_by_user($uid, $obj=TRUE) {
  $result = db_query("SELECT * FROM {imagepicker_user_groups} WHERE uid = %d", array($uid));
  if ($obj) {
    return db_fetch_object($result);
  }
  return db_fetch_array($result);
}

// imagepicker_group_images
// gid img_id

// build a table
function imagepicker_groups_list($myaccount=FALSE, $account=FALSE) {
  // show a table of the lists belonging to the current user
  if ($account) {
    $user = $account;
  }
  else {
    global $user;
  }
  if ($myaccount) {
    $editpath = 'user/'. $user->uid .'/imagepicker/groups/edit/';
    $deletepath = 'user/'. $user->uid .'/imagepicker/groups/delete/';
  }
  else {
    $editpath = "imagepicker/groups/edit/";
    $deletepath = "imagepicker/groups/delete/";
  }
  $output = "";
  $how_many = variable_get('imagepicker_rows_per_page', 25);
  $sql = "SELECT * FROM {imagepicker_user_groups} WHERE uid=". $user->uid ." ORDER BY group_name";
  $result = pager_query($sql, $how_many);
  $rows = array();
  $totcount = 0;
  $enabledlist = imagepicker_get_enabled_group();
  while ($row = db_fetch_array($result)) {
    $count = imagepicker_get_group_images_count($row['gid']);
    $totcount += $count;
    $row_data = array(
      check_plain($row['group_name']),
      check_plain($row['group_description']),
      $count,
      (($enabledlist && in_array($row['gid'], $enabledlist)) ? t('selected') : ''),
      l(t('Edit'), $editpath . $row['gid']),
      l(t('Delete'), $deletepath . $row['gid']),
    );
    $rows[] = $row_data;
  }

  if (count($rows)) {
    $header = array(
      t('Group name'),
      t('Description'),
      t('No. images'),
      t('State'),
      array('data' => t('Actions'), 'colspan' => 2),
    );
    $output .= '<div class="imgp_groups_list">'. theme('table', $header, $rows) . theme('pager', NULL, $how_many) ."</div>";

    $allcount = _imagepicker_user_has_img();
    $output .= '<div class="imgp_groups_info">';
    $output .= t('Total number of images: %allcount', array('%allcount' => $allcount)) .'<br/>';
    $output .= t('Total number of grouped images: %totcount', array('%totcount' => $totcount)) .'<br/>';
    $output .= t('Total number of ungrouped images: %lcount', array('%lcount' => $allcount-$totcount)) .'<br/>';
    $output .= '</div>';

    return $output;
  }
  else {
    return '<div class="messages">'. t('No groups found.') .'</div>';
  }
}
function imagepicker_get_group_images_count($gid) {
  $result = db_query("SELECT count(gid) as gidct FROM {imagepicker_group_images} WHERE gid = %d", array($gid));
  $row = db_fetch_array($result);
  return $row['gidct'];

}

function imagepicker_group_edit($gid) {
  $record = imagepicker_get_user_group($gid);
  $content .= drupal_get_form('imagepicker_groups_form', $record);
  return $content;
}

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
function imagepicker_group_delete_form_submit($form, &$form_state) {
  imagepicker_delete_user_group($form_state['values']['gid']);
}

// not in use
function imagepicker_get_grouped_images() {
  global $user;
  // now the enabled ones
  $result = db_query("
  SELECT i.img_id
  FROM {imagepicker_user_groups} g, {imagepicker_group_images} i
  WHERE g.uid=%d AND g.gid = i.gid AND g.state=1", array($user->uid));
  $ct = 0;
  while ($row = db_fetch_array($result)) {
    $data[] = $row['img_id'];
    $ct++;
  }
  if ($ct) {
    return $data;
  }
  return FALSE;
}

