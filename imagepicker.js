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

(function ($) {


/**
 * Hide the node form and show the busy div
*/
  Drupal.imagepicker_upload_progress_hide = function() {
    $('#imagepicker-upload-form').hide();
    $("#sending").show();
    $("#imagepicker_upload_progress_cancel_link").click(Drupal.imagepicker_upload_progress_show);
  }

  Drupal.imagepicker_upload_progress_hide_timeout = function() {
    var delay = Drupal.settings["imagepicker_upload_progress"]["delay"];
    setTimeout(Drupal.imagepicker_upload_progress_hide, delay*1000);
  }

  Drupal.imagepicker_upload_progress_show = function() {
    $('#imagepicker-upload-form').show();
    $("#sending").hide();

    // "reload" the form
    window.location = window.location;
  }

  Drupal.behaviors.imagepicker = function(context) {

    // Attaches the upload behaviour to the imagepicker upload form.
    $('#imagepicker-upload-form', context).submit(Drupal.imagepicker_upload_progress_hide_timeout);

    // exif info toggle
    $('#imgp_trig', context).click(function() {
      $('#imgp_targ', context).toggle('slow');
    });

    // form enhancement
    $("#edit-imagepicker-quota-byrole", context).change(function() {
      if ($(this).attr('checked')) {
        $("#wrap_imagepicker_quota_role", context).show();
      }
      else {
        $("#wrap_imagepicker_quota_role", context).hide();
      }
    });

    $("#edit-imagepicker-import-enabled", context).change(function() {
      if ($(this).attr('checked')) {
        $("#wrap-imagepicker-import", context).show();
      }
      else {
        $("#wrap-imagepicker-import", context).hide();
      }
    });

    $("#edit-imagepicker-exifinfo-enable", context).change(function() {
      if ($(this).attr('checked')) {
        $("#wrap-imagepicker-exifinfo-external", context).show();
      }
      else {
        $("#wrap-imagepicker-exifinfo-external", context).hide();
      }
    });

    $("#edit-imagepicker-upload-progress-enabled", context).change(function() {
      if ($(this).attr('checked')) {
        $("#wrap-imagepicker-upload-progress", context).show();
      }
      else {
        $("#wrap-imagepicker-upload-progress", context).hide();
      }
    });

    $("#edit-imagepicker-groups-enabled", context).change(function() {
      if ($(this).attr('checked')) {
        $("#wrap-imagepicker-groups", context).show();
      }
      else {
        $("#wrap-imagepicker-groups", context).hide();
      }
    });

    $("#edit-imagepicker-watermark-enable", context).change(function() {
      if ($(this).attr('checked')) {
        $("#wrap-imagepicker-watermark", context).show();
      }
      else {
        $("#wrap-imagepicker-watermark", context).hide();
      }
    });

  };

})(jQuery);
