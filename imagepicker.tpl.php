<?php
// $Id$
// $Name$
// for D7

/**
 * @file
 * template for imagepicker iframe
 */

?>
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-strict.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" lang="<?php print $language->language ?>" xml:lang="<?php print $language->language ?>">
<!-- imagepicker.tpl.php -->
<head>
  <title><?php echo $head_title ?></title>
  <?php echo $styles ?>
  <?php echo $scripts ?>
  <script type="text/javascript"><?php /* Needed to avoid Flash of Unstyle Content in IE */ ?> </script>
</head>
<body id="imagepicker" class="html not-front logged-in no-sidebars overlay" >
  <table border="0" cellpadding="0" cellspacing="0" id="content" width="100%">
  <tr>
    <td valign="top">
      <div>
        <div class="tabs">
          <?php
           if (is_array($tabs) ) {
             echo render($tabs);
           }
           else {
             echo $tabs;
           }
           ?>
        </div>
        <div id="imgp">
          <?php echo $messages ?>
          <?php echo $content; ?>
        </div>
      </div>
    </td>
  </tr>
  </table>
</body>
</html>
