<?php
// $Id$
// $Name$
// for D6. Version 2
/**
 * @file
 * Themimg functions for imagepicker.
 */

function theme_imagepicker(&$content) {
  global $language;
  $path = drupal_get_path('module', 'imagepicker');
  drupal_add_css($path .'/imagepicker.css');
  $vars['content'] = $content;
  $vars['head_title']  = (drupal_get_title() ? strip_tags(drupal_get_title()) : variable_get('site_name', 'Drupal'));
  $vars['styles'] = drupal_get_css();
  $vars['scripts'] = drupal_get_js();
  $vars['tabs'] = theme('menu_local_tasks');
  $vars['messages'] = imagepicker_strip_messages(theme('status_messages'));
  $vars['language'] = $language;
  $template = 'imagepicker-page.tpl.php';
  $defaulttemplate = variable_get('theme_default', '');
  if ($defaulttemplate) {
    $templatepath = drupal_get_path('theme', $defaulttemplate);
    if (file_exists($templatepath .'/'. $template)) {
      $template = $templatepath .'/'. $template;
    }
    else {
      $template = $path . '/' . $template;
    }
  }
  echo theme_render_template($template, $vars);
  exit;
}

function theme_imagepicker_list($header, $rows, $max, $message="", $pref="", $suff="", $label="") {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  if ($rows) {
    $pager = theme('pager', NULL, $max);
    $output .= $pref . theme('table', $header, $rows) . $pager . $suff;
  }
  else {
    $output .= '<div class="messages">'. $message .'</div>';
  }
  $output .= ($label ? '</fieldset>' : '');
  return $output;

}

function theme_imagepicker_stats($header, $rows, $pref="", $suff="", $label="") {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= $pref . theme('table', $header, $rows) . $suff;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_browser($content, $forms='', $message='', $help='', $label='') {
  $rows = $content[0];
  $max = $content[1];
  $cols = $content[2];
  $pref1 = $content[3][0];
  $pref2 = $content[3][1];
  $suff1 = $content[4][0];
  $suff2 = $content[4][1];
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  if ($rows) {
    // forms
    if ($forms) {
      foreach ($forms AS $form) {
        $output .= $form;
      }
    }
    // help message
    if ($help) {
      $output .= '<div class="imgp_help">'. $help . '</div>';
    }
    $pager = theme('pager', NULL, $max);
    $output .= $pref1;
    $ct = 0;
    foreach ($rows AS $row) {
      $tooltip =  $row['img_name'] .': '. $row['img_title'] .' '. $row['img_description'];
      $imglink = '<img src="'. $row['imgpath']  .'" alt="'. $row['img_title'] .'" title="'. $tooltip .'" />';
      $output .= $pref2 . l($imglink, $row['imgurl'] . $row['img_id'], array('html' => TRUE)) . $suff2;
      $ct++;
      if ($cols > 0 && $ct >= $cols) {
        $output .= $suff1 . $pref1;
        $ct = 0;
      }
    }
    $output .= $suff1 . $pager;
  }
  else {
    $output .= '<div class="messages">'. $message .'</div>';
  }
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_fullpage($image, $source, $link) {
  $output = '<div id="imgp_page"><div id="imgp_page_img">';
  $output .= '<a href="'. $source .'" alt="'. $image['img_title'] .'" target="_blank">';
  $output .= '<img src="'. $source .'" alt="'. $image['img_title'] .'" /></a></div>';
  $output .= ($image['img_description'] ? '<div>'. nl2br($image['img_description']) .'</div>' : '');
  $output .= ($link ? '<div><a href="#" onclick="history.back()">'. t('Return to page') .'</a></div>' : '');
  $output .= '</div>';
  return $output;
}

function theme_imagepicker_quota_message($message1, $message2, $form, $label, $help) {
  $output = ($help ? '<div class="imgp_help">' . $help . '</div>' : '');
  $output .= ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= ($message1 ? '<p class="messages">'. $message1 .'</p>' : '') ;
  $output .= $form;
  $output .= ($message2 ? '<p class="messages">'. $message2 .'</p>' : '') ;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

function theme_imagepicker_view($img, $imgpath, $info) {
  $output = '<div id="imgp_img_view">';
  $output .= ($img['img_title'] ? '<div id="imgp_img_view_title">'. $img['img_title'] .'</div>' : '');
  $output .= '<img id="imgp_img_view_img" src="'. $imgpath .'" alt="'. $img['img_title'] .'" title="'. $img['img_name'] .'" />';
  $output .= ($img['img_description'] ? '<div id="imgp_img_view_desc">'. nl2br($img['img_description']) .'</div>' : '');
  $output .= '<div>';
  $output .= t('Width')  .": ". $info['width']     ."&nbsp;&nbsp;&nbsp;";
  $output .= t('Height') .": ". $info['height']    ."&nbsp;&nbsp;&nbsp;";
  $output .= t('Type')   .": ". $info['extension'] ."&nbsp;&nbsp;&nbsp;";
  $output .= t('Size')   .": ". $info['file_size'] . '</div></div>';
  return $output;
}

function theme_imagepicker_image_edit_header($image, $source) {
  $output = '<div class="imgp_help">'. t('Edit image details') . '</div>';
  $output .= '<div id="imgp_img_holder"><img src="'. $source .'" alt="'. check_plain($image['img_title']) .'" /></div>';
  return $output;
}

function theme_imagepicker_quota( $form, $message, $label) {
  $output = ($label ? '<fieldset><legend>'. $label .'</legend>' : '');
  $output .= ($message ? '<p class="messages">'. $message .'</p>' : '') ;
  $output .= $form;
  $output .= ($label ? '</fieldset>' : '');
  return $output;
}

