<?php

/**
 * @file
 * Creates a default view of programs.
 */

/**
 * Implements hook_views_default_views().
 */
function pbs_media_manager_program_views_default_views() {
  $view = new view();
  $view->name = 'pbs_media_manager_programs';
  $view->description = '';
  $view->tag = 'pbs_media_manager';
  $view->base_table = 'pbs_media_manager_programs';
  $view->human_name = 'pbs_media_manager Programs';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'pbs_media_manager Programs List';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'view_pbs_media_manager_programs';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'default';
  $handler->display->display_options['row_plugin'] = 'fields';
  $handler->display->display_options['row_options']['inline'] = array(
    'iphone_small_img_url' => 'iphone_small_img_url',
  );
  /* Header: Global: Text area */
  $handler->display->display_options['header']['area']['id'] = 'area';
  $handler->display->display_options['header']['area']['table'] = 'views';
  $handler->display->display_options['header']['area']['field'] = 'area';
  $handler->display->display_options['header']['area']['empty'] = TRUE;
  $handler->display->display_options['header']['area']['content'] = '<a href="/admin/config/media/pbs_programs/import">Import Programs<a>';
  $handler->display->display_options['header']['area']['format'] = 'full_html';
  /* Field: pbs_media_manager Program: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = '';
  $handler->display->display_options['fields']['title']['element_type'] = 'h2';
  $handler->display->display_options['fields']['title']['element_label_colon'] = FALSE;
  /* Field: pbs_media_manager Program: Program_mezzanine_16x9_img_url */
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['id'] = 'program_mezzanine_16x9_img_url_1';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['field'] = 'program_mezzanine_16x9_img_url';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['label'] = '';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['alter']['alter_text'] = TRUE;
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['alter']['text'] = '<img src="[program_mezzanine_16x9_img_url_1].resize.200x200.jpg">';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url_1']['element_label_colon'] = FALSE;
  /* Field: pbs_media_manager Program: Short_description */
  $handler->display->display_options['fields']['short_description']['id'] = 'short_description';
  $handler->display->display_options['fields']['short_description']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['short_description']['field'] = 'short_description';
  $handler->display->display_options['fields']['short_description']['label'] = '';
  $handler->display->display_options['fields']['short_description']['element_label_colon'] = FALSE;
  /* Field: pbs_media_manager Program: Resource_uri */
  $handler->display->display_options['fields']['resource_uri']['id'] = 'resource_uri';
  $handler->display->display_options['fields']['resource_uri']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['resource_uri']['field'] = 'resource_uri';
  $handler->display->display_options['fields']['resource_uri']['label'] = 'pbs_media_manager URI';
  /* Field: pbs_media_manager Program: Program_logo_wide_img_url */
  $handler->display->display_options['fields']['program_logo_wide_img_url']['id'] = 'program_logo_wide_img_url';
  $handler->display->display_options['fields']['program_logo_wide_img_url']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['program_logo_wide_img_url']['field'] = 'program_logo_wide_img_url';
  /* Field: pbs_media_manager Program: Program_mezzanine_16x9_img_url */
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url']['id'] = 'program_mezzanine_16x9_img_url';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['program_mezzanine_16x9_img_url']['field'] = 'program_mezzanine_16x9_img_url';
  /* Field: pbs_media_manager Program: Program_poster_2x3_img_url */
  $handler->display->display_options['fields']['program_poster_2x3_img_url']['id'] = 'program_poster_2x3_img_url';
  $handler->display->display_options['fields']['program_poster_2x3_img_url']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['program_poster_2x3_img_url']['field'] = 'program_poster_2x3_img_url';
  /* Field: pbs_media_manager Program: Program_banner_img_url */
  $handler->display->display_options['fields']['program_banner_img_url']['id'] = 'program_banner_img_url';
  $handler->display->display_options['fields']['program_banner_img_url']['table'] = 'pbs_media_manager_programs';
  $handler->display->display_options['fields']['program_banner_img_url']['field'] = 'program_banner_img_url';

  /* Display: pbs_media_manager Programs Entity Listing Page */
  $handler = $view->new_display('page', 'pbs_media_manager Programs Entity Listing Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/pbs_media_manager-programs';
  $handler->display->display_options['menu']['type'] = 'normal';
  $handler->display->display_options['menu']['title'] = 'pbs_media_manager Program Entities';
  $handler->display->display_options['menu']['weight'] = '0';
  $handler->display->display_options['menu']['name'] = 'management';
  $handler->display->display_options['menu']['context'] = 0;
  $handler->display->display_options['menu']['context_only_inline'] = 0;

}
