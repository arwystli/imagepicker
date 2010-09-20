<?php
// $Id$
// $Name$
// for D7

/**
 * @file
 * Themimg functions for imagepicker.
 */

/**
 * theme registry
 */
function imagepicker_theme() {

  return array(
    'imagepicker' => array(
      'variables' => array('content' => NULL),
      'template' => 'imagepicker',
    ),
    'imagepicker_iframe' => array(
      'variables' => array(
        'content' => NULL,
        'img' => NULL,
        'public' => NULL,
      ),
    ),
    'imagepicker_list' => array(
      'variables' => array(
        'header' => array(),
        'rows' => NULL,
        'max' => 50,
        'message' => NULL,
        'pref' => NULL,
        'suff' => NULL,
        'label' => NULL,
        'forms' => NULL,
      ),
    ),
    'imagepicker_stats' => array(
      'variables' => array(
        'header' => array(),
        'rows' => array(),
        'message' => NULL,
        'pref' => "",
        'suff' => "",
        'label' => "",
      ),
    ),
    'imagepicker_browser' => array(
      'variables' => array(
        'content' => array(),
        'forms' => array(),
        'message' => "",
        'help' => "",
        'label' => "",
      ),
    ),
    'imagepicker_fullpage' => array(
      'variables' => array(
        'image' => NULL,
        'source' => "",
        'link' => 1,
      ),
    ),
    'imagepicker_quota_message' => array(
      'variables' => array(
        'message1' => "",
        'message2' => "",
        'form' => array(),
        'label' => "",
        'help' => "",
      ),
    ),
    'imagepicker_view' => array(
      'variables' => array(
        'img' => array(),
        'imgpath' => "",
        'info' => array(),
        'exifinfo' => array(),
      ),
    ),
    'imagepicker_image_edit_header' => array(
      'variables' => array(
        'image' => array(),
        'source' => "",
      ),
    ),
    'imagepicker_quota' => array(
      'variables' => array(
        'quotaform' => '',
        'message' => "",
        'label' => "",
      ),
    ),
    'imagepicker_upload_progress_message' => array(
      'variables' => array(
        'message' => "",
      ),
    ),
    'imagepicker_user_config' => array(
      'variables' => array(
        'form' => array(),
        'label' => "",
        'help' => "",
        'message1' => "",
        'message2' => "",
      ),
    ),
    'imagepicker_user_view' => array(
      'variables' => array(
        'view' => "",
        'form1' => array(),
        'form2' => array(),
        'form3' => array(),
      ),
    ),
    'imagepicker_admin_view' => array(
      'variables' => array(
        'view' => "",
        'form1' => array(),
        'form2' => array(),
        'form3' => array(),
      ),
    ),
    'imagepicker_admin_images' => array(
      'variables' => array(
        'view' => "",
        'form1' => array(),
        'form2' => array(),
        'form3' => array(),
      ),
    ),
    'imagepicker_insert' => array(
      'variables' => array(
        'img' => NULL,
        'public' => NULL,
        'form1' => array(),
        'form2' => array(),
      ),
    ),
    'imagepicker_usert_search_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_groupt_search_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_groupt_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_general_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_browset_search_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_browset_public_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_browset_public_groups_form' => array(
      'render element' => 'form'
    ),
    'imagepicker_browset_order_form' => array(
      'render element' => 'form'
    ),
  );
}

// set up the iframe
function theme_imagepicker_iframe($variables) {
  $content = $variables['content'];
  $img = (isset($variables['img']) ? $variables['img'] : '');
  $public = (isset($variables['public']) ? $variables['public'] : FALSE);

  drupal_add_css(IMAGEPICKER_PATH .'/imagepicker.css');
  if ($img) {
    global $user;
    $img_title = check_plain($img->img_title);
    $settings = array(
      'imagepicker_iframe' => array(
        'imgpImageAlt' => ($img_title ? addslashes($img_title) : t('Image') ),
        'imgpImageTitle' => ($img_title ? addslashes($img_title) : ''),
        'imgpImageDesc' => preg_replace("/(\n|\r)/", '', nl2br($img->img_description)),
        'imgpFileLink' => imagepicker_get_image_path($img, 'full', ($public ? array('name' => $img->name, 'uid' => $img->uid ) : FALSE) ),
        'imgpThumbLink' => imagepicker_get_image_path($img, 'thumb', ($public ? array('name' => $img->name, 'uid' => $img->uid ) : FALSE)),
        'imgpPageLink' => url('imagepicker/image/' . $img->img_id),
        'isFCKeditor' => (module_exists('fckeditor') ? 'yes' : 'no' ),
        'isWysiwyg' => (module_exists('wysiwyg') ? 'yes' : 'no' ),
        'use_cssbox' => ((variable_get('imagepicker_use_cssbox', 0) && ! isset($user->data['imagepicker_use_cssbox']) ) ? 1 : (variable_get('imagepicker_use_cssbox', 0) && isset($user->data['imagepicker_use_cssbox']) && $user->data['imagepicker_use_cssbox']) ? 1 : 0),
        'default_align_show' => ((variable_get('imagepicker_default_align_show', 1) && ! isset($user->data['imagepicker_default_align_show']) ) ? 1 : (variable_get('imagepicker_default_align_show', 1) && isset($user->data['imagepicker_default_align_show']) && $user->data['imagepicker_default_align_show']) ? 1 : 0),
        'default_fleft' => (isset($user->data['imagepicker_default_fleft']) ? $user->data['imagepicker_default_fleft'] : variable_get('imagepicker_default_fleft', 'style="float: left"')),
        'default_fright' => (isset($user->data['imagepicker_default_fright']) ? $user->data['imagepicker_default_fright'] : variable_get('imagepicker_default_fright', 'style="float: right"')),
        'lightbox2_enable' => (module_exists('lightbox2') && variable_get('imagepicker_lightbox2_enable', 1)),
        'lightbox2_insert' => (isset($user->data['imagepicker_lightbox2_insert']) ? $user->data['imagepicker_lightbox2_insert'] : variable_get('imagepicker_lightbox2_insert', 'lightbox')),
        'colorbox_enable' => (module_exists('colorbox') && variable_get('imagepicker_colorbox_enable', 0)),
        'node_editbody' => 'edit-body-' . variable_get('imagepicker_node_lang', '') . '-' . variable_get('imagepicker_node_pos', 0) . '-value',
      ),
    );
    drupal_add_js($settings, 'setting');
    drupal_add_js(IMAGEPICKER_PATH .'/imagepicker_iframe.js');
  }
  return $content;
}

function theme_imagepicker_list($variables) {

  // header, rows, max, message, pref, suff label, forms
  $output = ($variables['label']  ? '<fieldset><legend>' . $variables['label'] . '</legend>' : '');
  if ($variables['rows'] ) {
    // only show the forms if there is something there
    if ($variables['forms']) {
      if ($variables['forms']['browse_public']) {
        $form = theme('imagepicker_browset_public_form', array('form' => $variables['forms']['browse_public']));
        $output .= $form;
      }
      if ($variables['forms']['browse_public_groups']) {
        $form = theme('imagepicker_browset_public_groups_form', array('form' => $variables['forms']['browse_public_groups']));
        $output .= $form;
      }
      if ($variables['forms']['groups']) {
        $form = theme('imagepicker_groupt_form', array('form' => $variables['forms']['groups']));
        $output .= $form;
      }
      if ($variables['forms']['search']) {
        $form = theme('imagepicker_browset_search_form', array('form' => $variables['forms']['search']));
        $output .= $form;
      }
    }

    $output .= $variables['pref'] . theme('table', array('header' => $variables['header'], 'rows' => $variables['rows'])) . theme('pager') . $variables['suff'];
  }
  else {
    $output .= '<div class="messages">' . $variables['message'] . '</div>';
  }
  $output .= ($variables['label'] ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_stats($variables) {

  $output = ($variables['label'] ? '<fieldset><legend>' . $variables['label'] . '</legend>' : '');
  if (count($variables['rows']) ) {
    $output .=
      $variables['pref'] .
      theme('table', array('header' => $variables['header'], 'rows' => $variables['rows']) ) .
      $variables['suff'];
  }
  else {
    $output .= '<div class="messages">' . $variables['message'] . '</div>';
  }
  $output .= ($variables['label'] ? '</fieldset>' : '');
  return $output;
}

// theme for thumbnail browser
function theme_imagepicker_browser($variables) {

  $content = $variables['content'];
  $forms = $variables['forms'];
  $message = $variables['message'];
  $help = $variables['help'];
  $label = $variables['label'];

  $rows = $content[0];
  $max = $content[1];
  $cols = $content[2];
  $pref1 = $content[3][0];
  $pref2 = $content[3][1];
  $suff1 = $content[4][0];
  $suff2 = $content[4][1];
  $output = ($label ? '<fieldset><legend>' . $label . '</legend>' : '');
  if ($rows) {
    // forms
    if ($forms) {
      // browse_order browse_public browse_search
      if (isset($forms['browse_order'])) {
        $form = theme('imagepicker_browset_order_form', array('form' => $forms['browse_order']));
        $output .= $form;
      }
      if (isset($forms['browse_public'])) {
        $form = theme('imagepicker_browset_public_form', array('form' => $forms['browse_public']));
        $output .= $form;
      }
      if (isset($forms['browse_public_groups'])) {
        $form = theme('imagepicker_browset_public_groups_form', array('form' => $forms['browse_public_groups']));
        $output .= $form;
      }
      if (isset($forms['browse_groups'])) {
        $form = theme('imagepicker_groupt_form', array('form' => $forms['browse_groups']));
        $output .= $form;
      }
      if (isset($forms['browse_search'])) {
        $form = theme('imagepicker_browset_search_form', array('form' => $forms['browse_search']));
        $output .= $form;
      }
    }
    // help message
    $output .= ($help ? '<div class="imgp_help">' . $help . '</div>' : '');
    $output .= $pref1;
    $ct = 0;
    foreach ($rows AS $row) {
      $tooltip =  $row['img_name'] . ': ' . $row['img_title'] . ' ' . $row['img_description'];
      $imglink = '<img src="' . $row['imgpath'] . '" alt="' . $row['img_title'] . '" title="' . $tooltip . '" />';
      $output .= $pref2 . l($imglink, $row['imgurl'] . $row['img_id'], array('html' => TRUE)) . $suff2;
      $ct++;
      if ($cols > 0 && $ct >= $cols) {
        $output .= $suff1 . $pref1;
        $ct = 0;
      }
    }
    $output .= $suff1 . theme('pager');
  }
  else {
    $output .= '<div class="messages">' . $message . '</div>';
  }
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_fullpage($variables) {

  $image = $variables['image'];
  $source = $variables['source'];
  $link = $variables['link'];
  $output = '<div id="imgp_page"><div id="imgp_page_img">';
  $output .= '<a href="' . $source . '" alt="' . (isset($image->img_title) ? $image->img_title : '') . '" target="_blank">';
  $output .= '<img src="' . $source . '" alt="' . (isset($image->img_title) ? $image->img_title : '') . '" /></a></div>';
  $output .= (isset($image->img_description) ? '<div>' . nl2br($image->img_description) . '</div>' : '');
  $output .= ($link ? '<div><a href="#" onclick="history.back()">' . t('Return to page') . '</a></div>' : '');
  $output .= '</div>';
  return $output;
}

function theme_imagepicker_quota_message($variables) {

  $message1 = $variables['message1'];
  $message2 = $variables['message2'];
  $form = $variables['form'];
  $label = $variables['label'];
  $help = $variables['help'];
  $output = ($label ? '<fieldset><legend>' . $label . '</legend>' : '');
  $output .= ($message1 ? '<p class="messages">' . $message1 . '</p>' : '') ;
  $output .= ($help ? '<div class="imgp_help">' . $help . '</div>' : '');
  if (is_array($form) ) {
    $output .= drupal_render($form);
  }
  else {
    $output .= $form;
  }
  $output .= ($message2 ? '<p class="messages">' . $message2 . '</p>' : '') ;
  $output .= ($label ? '</fieldset>' : '');

  return $output;
}

function theme_imagepicker_view($variables) {

  $img = $variables['img'];
  $imgpath = $variables['imgpath'];
  $info = $variables['info'];
  $exifinfo = $variables['exifinfo'];
  $output = '<div id="imgp_img_view">';
  $output .= ($img->img_title ? '<div id="imgp_img_view_title">' . $img->img_title . '</div>' : '');
  $output .= '<img id="imgp_img_view_img" src="' . $imgpath . '" alt="' . $img->img_title . '" title="' . $img->img_name . '" />';
  $output .= ($img->img_description ? '<div id="imgp_img_view_desc">' . nl2br($img->img_description) . '</div>' : '');
  $output .= '<div>';
  $output .= t('Width') . ": " . $info['width'] . "&nbsp;&nbsp;&nbsp;";
  $output .= t('Height') . ": " . $info['height'] . "&nbsp;&nbsp;&nbsp;";
  $output .= t('Type') . ": " . $info['extension'] . "&nbsp;&nbsp;&nbsp;";
  $output .= t('Size') . ": " . $info['file_size'];
  $output .= '</div>';

  if ($exifinfo) {
    $output .= '<a id="imgp_trig">' . t('Show/Hide Exif Info') . '</a>';
    $output .= '<div id="imgp_targ" >';
    if (is_array($exifinfo)) {
      // using php function exif_read_data().
      foreach ($exifinfo AS $key => $section) {
        $header = array(
          array('data' => drupal_ucfirst(drupal_strtolower($key)), 'colspan' => 2),
        );
        $rows = array();
        foreach ($section as $name => $val) {
          if ($key == 'COMMENT') {
            $a = explode(':', $val);
            $rows[] = array($a[0] . ':', $a[1]);
          }
          else {
            $rows[] = array($name . ': ', $val);
          }
        }
        $output .= theme('table', array('header' => $header, 'rows' => $rows));
        unset($header);
        unset($rows);
      }
    }
    else {
      // from an external source eg exiftool
      $output .= '<pre>' . check_plain($exifinfo) . '</pre>';
    }
    $output .= '</div>';
  }
  $output .= '</div>';
  return $output;
}

function theme_imagepicker_image_edit_header($variables) {

  $image = $variables['image'];
  $source = $variables['source'];
  $output = '<div class="imgp_help">' . t('Edit image details') . '</div>';
  $output .= '<div id="imgp_img_holder"><img src="' . $source . '" alt="' . check_plain($image->img_title) . '" /></div>';
  return $output;
}

function theme_imagepicker_quota($variables) {

  $form = $variables['quotaform'];
  $message = $variables['message'];
  $label = $variables['label'];
  $output = ($label ? '<fieldset><legend>' . $label . '</legend>' : '');
  $output .= ($message ? '<p class="messages">' . $message . '</p>' : '') ;
  $output .= drupal_render($form);
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_upload_progress_message($variables) {

  $output = '<div id="sending" class="progress" style="display: none;">';
  $output .= '<div class="status">' . $variables['message'] . '</div>';
  $output .= '<div class="bar"></div></div>';
  return $output;
}

function theme_imagepicker_user_config($variables) {

  $form = $variables['form'];
  $label = $variables['label'];
  $help = $variables['help'];
  $message1 = $variables['message1'];
  $message2 = $variables['message2'];

  $output = ($label ? '<fieldset><legend>' . $label . '</legend>' : '');
  $output .= ($help ? '<div class="imgp_help">' . $help . '</div>' : '');
  $output .= ($message1 ? '<p class="messages">' . $message1 . '</p>' : '') ;
  $output .= drupal_render($form);
  $output .= ($message2 ? '<p class="messages">' . $message2 . '</p>' : '') ;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_user_view($variables) {

  $view = $variables['view'];
  $form1 = $variables['form1'];
  $form2 = $variables['form2'];
  $form3 = $variables['form3'];

  $output = "";
  $output .= $view;
  $output .= '<br />';
  $output .= ($form1 ? drupal_render($form1) : '') ;
  $output .= '<br />';
  $output .= ($form2 ? drupal_render($form2) : '') ;
  $output .= ($form3 ? drupal_render($form3) : '') ;
  return $output;
}

function theme_imagepicker_admin_view($variables) {

  $view = $variables['view'];
  $form1 = $variables['form1'];
  $form2 = $variables['form2'];
  $form3 = $variables['form3'];

  $output = "";
  $output .= $view;
  $output .= '<br />';
  $output .= ($form1 ? drupal_render($form1) : '') ;
  $output .= '<br />';
  $output .= ($form2 ? drupal_render($form2) : '') ;
  $output .= ($form3 ? drupal_render($form3) : '') ;
  return $output;
}

function theme_imagepicker_admin_images($variables) {

  $view = $variables['view'];
  $form1 = $variables['form1'];
  $form2 = $variables['form2'];
  $form3 = $variables['form3'];

  $output = "";
  $output .= $view;
  $output .= '<br />';
  $output .= ($form1 ? drupal_render($form1) : '') ;
  $output .= '<br />';
  $output .= ($form2 ? drupal_render($form2) : '') ;
  $output .= ($form3 ? drupal_render($form3) : '') ;
  return $output;
}

function theme_imagepicker_insert($variables) {

  $img = $variables['img'];
  $public = $variables['public'];
  $form1 = $variables['form1'];
  $form2 = $variables['form2'];

  $output = "";
  if ($img) {
    $imgpath = imagepicker_get_image_path($img, 'browser', ($public ? array('name' => $img->name , 'uid' => $img->uid) : FALSE ));
    $output .= '<div class="imgp_help">' . t('Choose the settings you want, place the cursor in the Body box above and Insert image.') . '</div>';
    if (isset($img->img_title)) {
      $output .= '<div id="imgp_img_holder_title">' . check_plain($img->img_title) . '</div>';
    }
    if (isset($img->img_description)) {
      $output .= '<div id="imgp_img_holder_description">' . nl2br($img->img_description) . '</div>';
    }
    $output .= '<div id="imgp_img_holder">';
    $output .= '<img id="imgp_img" src="' . $imgpath . '" alt="' . (isset($img->img_title) ? check_plain($img->img_title) : t('Image') ) . '" title="' . $img->img_name . '" />';
    $output .= '</div>';
    if (is_array($form1)) {
      $output .= drupal_render($form1);
    }
    if (is_array($form2)) {
      $output .= drupal_render($form2);
    }
  }
  return $output;
}

function theme_imagepicker_usert_search_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= '<div id="imgp_users_form" class="container-inline">';
    $output .= drupal_render($form);
    $output .= '</div>';
  }
  return $output;
}

function theme_imagepicker_groupt_search_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= '<div id="imgp_users_form" class="container-inline">';
    $output .= drupal_render($form);
    $output .= '</div>';
  }
  return $output;
}

function theme_imagepicker_browset_search_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= '<div id="imgp_search_form" class="container-inline">';
    $output .= drupal_render($form);
    $output .= '</div>';
  }
  return $output;
}

function theme_imagepicker_browset_public_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= '<div id="imgp_browse_form" class="container-inline">';
    $output .= drupal_render($form);
    $output .= '</div>';
  }
  return $output;
}

function theme_imagepicker_browset_public_groups_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= '<div id="imgp_browse_form" class="container-inline">';
    $output .= drupal_render($form);
    $output .= '</div>';
  }
  return $output;
}

function theme_imagepicker_browset_order_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= '<div id="imgp_order_form" class="container-inline">';
    $output .= drupal_render($form);
    $output .= '</div>';
  }
  return $output;
}

function theme_imagepicker_groupt_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= drupal_render($form);
  }
  return $output;
}

function theme_imagepicker_general_form($variables) {

  $form = $variables['form'];
  $output = '';
  if (is_array($form)) {
    $output .= drupal_render($form);
  }
  return $output;
}

// preprocess for imagepicker.tpl.php
function template_preprocess_imagepicker(&$variables) {
  global $language;
  if (module_exists('admin_menu')) {
    admin_menu_suppress();
  }

  $variables['head_title']  = (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal'));
  $variables['styles'] = drupal_get_css();
  $variables['scripts'] = drupal_get_js();
  // fix jquery version thing in the iframe
  if (module_exists('jquery_update')) {
    jquery_update_preprocess_page($variables);
  }
  $variables['tabs'] = theme('menu_local_tasks');
  $variables['messages'] = imagepicker_strip_messages(theme('status_messages'));
  $variables['language'] = $language;

}
