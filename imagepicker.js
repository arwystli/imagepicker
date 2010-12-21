// $Id$
/**
 * @file
 * Javascript functions for admin form enhancement
 *
*/

(function ($) {
  Drupal.behaviors.imagepicker = function(context) {

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
