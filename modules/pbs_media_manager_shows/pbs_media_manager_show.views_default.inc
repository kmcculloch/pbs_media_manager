<?php

/**
 * @file
 * Creates a default view of shows.
 */

/**
 * Implements hook_views_default_views().
 */
function pbs_media_manager_show_views_default_views() {
  $view = new view();
  $view->name = 'pbs_show_list';
  $view->description = '';
  $view->tag = 'default';
  $view->base_table = 'pbs_media_manager_shows';
  $view->human_name = 'PBS Show List';
  $view->core = 7;
  $view->api_version = '3.0';
  $view->disabled = FALSE; /* Edit this to true to make a default view disabled initially */

  /* Display: Master */
  $handler = $view->new_display('default', 'Master', 'default');
  $handler->display->display_options['title'] = 'PBS Show List';
  $handler->display->display_options['use_more_always'] = FALSE;
  $handler->display->display_options['access']['type'] = 'perm';
  $handler->display->display_options['access']['perm'] = 'access content overview';
  $handler->display->display_options['cache']['type'] = 'none';
  $handler->display->display_options['query']['type'] = 'views_query';
  $handler->display->display_options['exposed_form']['type'] = 'basic';
  $handler->display->display_options['pager']['type'] = 'full';
  $handler->display->display_options['pager']['options']['items_per_page'] = '50';
  $handler->display->display_options['style_plugin'] = 'table';
  /* Field: PBS Show: Pbs show ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  /* Field: PBS Show: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Title';
  /* Field: PBS Show: Nola_root */
  $handler->display->display_options['fields']['nola_root']['id'] = 'nola_root';
  $handler->display->display_options['fields']['nola_root']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['nola_root']['field'] = 'nola_root';
  /* Field: PBS Show: Slug */
  $handler->display->display_options['fields']['slug']['id'] = 'slug';
  $handler->display->display_options['fields']['slug']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['slug']['field'] = 'slug';
  /* Sort criterion: PBS Show: Label */
  $handler->display->display_options['sorts']['title']['id'] = 'title';
  $handler->display->display_options['sorts']['title']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['sorts']['title']['field'] = 'title';

  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/pbs-shows';

}
