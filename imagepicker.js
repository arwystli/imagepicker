// $Id$
// Original Id: upload_progress.js,v 1.1 2007/11/17 06:47:47 pfournier Exp
/**
 * @file
 * Javascript functions for progress bar status on node creation forms
 *
 * @author Patrick Fournier <patrick at patrickfournier dot com>
 * @author Fabio Varesano <fvaresano at yahoo dot it>
 * Adapted by Bob Hutchinson for imagepicker upload form
*/

/**
 * Hide the node form and show the busy div
*/
Drupal.imagepicker_upload_progress_hide_timeout = function() {
  var delay = Drupal.settings["imagepicker_upload_progress"]["delay"];
  setTimeout('imagepicker_upload_progress_hide()', delay*1000);
}

function imagepicker_upload_progress_hide() {
  $('#imagepicker-upload-form').hide();
  $("#sending").show();
  $("#imagepicker_upload_progress_cancel_link").click(Drupal.imagepicker_upload_progress_show);
}

Drupal.imagepicker_upload_progress_show = function() {
  $('#imagepicker-upload-form').show();
  $("#sending").hide();

  // "reload" the form
  window.location = window.location;
}

/**
 * Attaches the upload behaviour to the imagepicker upload form.
 */
Drupal.imagepicker_upload_progress = function() {
  $('#imagepicker-upload-form').submit(Drupal.imagepicker_upload_progress_hide_timeout);
}

/**
 * A toggle function for imagepicker.
 */
Drupal.imagepicker_toggle = function() {
  $('#imgp_trig').click(function() {
    $('#imgp_targ').toggle('slow');
  });
}

// Global killswitch
if (Drupal.jsEnabled) {
  $(document).ready(Drupal.imagepicker_upload_progress);
  $(document).ready(Drupal.imagepicker_toggle);
}
