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
  $handler->display->display_options['style_options']['columns'] = array(
    'id' => 'id',
    'title' => 'title',
    'nola_root' => 'nola_root',
    'slug' => 'slug',
    'genre_title' => 'genre_title',
  );
  $handler->display->display_options['style_options']['default'] = 'title';
  $handler->display->display_options['style_options']['info'] = array(
    'id' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'nola_root' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'slug' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
    'genre_title' => array(
      'sortable' => 1,
      'default_sort_order' => 'asc',
      'align' => '',
      'separator' => '',
      'empty_column' => 0,
    ),
  );
  /* Field: PBS Show: Pbs show ID */
  $handler->display->display_options['fields']['id']['id'] = 'id';
  $handler->display->display_options['fields']['id']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['id']['field'] = 'id';
  $handler->display->display_options['fields']['id']['label'] = 'PBS Show ID';
  /* Field: PBS Show: Label */
  $handler->display->display_options['fields']['title']['id'] = 'title';
  $handler->display->display_options['fields']['title']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['title']['field'] = 'title';
  $handler->display->display_options['fields']['title']['label'] = 'Title';
  /* Field: PBS Show: Nola_root */
  $handler->display->display_options['fields']['nola_root']['id'] = 'nola_root';
  $handler->display->display_options['fields']['nola_root']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['nola_root']['field'] = 'nola_root';
  $handler->display->display_options['fields']['nola_root']['label'] = 'NOLA Root';
  /* Field: PBS Show: Slug */
  $handler->display->display_options['fields']['slug']['id'] = 'slug';
  $handler->display->display_options['fields']['slug']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['slug']['field'] = 'slug';
  /* Field: PBS Show: Genre_title */
  $handler->display->display_options['fields']['genre_title']['id'] = 'genre_title';
  $handler->display->display_options['fields']['genre_title']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['fields']['genre_title']['field'] = 'genre_title';
  $handler->display->display_options['fields']['genre_title']['label'] = 'Genre';
  /* Filter criterion: PBS Show: Label */
  $handler->display->display_options['filters']['title']['id'] = 'title';
  $handler->display->display_options['filters']['title']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['filters']['title']['field'] = 'title';
  $handler->display->display_options['filters']['title']['operator'] = 'contains';
  $handler->display->display_options['filters']['title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['title']['expose']['operator_id'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['label'] = 'Title';
  $handler->display->display_options['filters']['title']['expose']['operator'] = 'title_op';
  $handler->display->display_options['filters']['title']['expose']['identifier'] = 'title';
  $handler->display->display_options['filters']['title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: PBS Show: Nola_root */
  $handler->display->display_options['filters']['nola_root']['id'] = 'nola_root';
  $handler->display->display_options['filters']['nola_root']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['filters']['nola_root']['field'] = 'nola_root';
  $handler->display->display_options['filters']['nola_root']['exposed'] = TRUE;
  $handler->display->display_options['filters']['nola_root']['expose']['operator_id'] = 'nola_root_op';
  $handler->display->display_options['filters']['nola_root']['expose']['label'] = 'NOLA Root';
  $handler->display->display_options['filters']['nola_root']['expose']['operator'] = 'nola_root_op';
  $handler->display->display_options['filters']['nola_root']['expose']['identifier'] = 'nola_root';
  $handler->display->display_options['filters']['nola_root']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: PBS Show: Slug */
  $handler->display->display_options['filters']['slug']['id'] = 'slug';
  $handler->display->display_options['filters']['slug']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['filters']['slug']['field'] = 'slug';
  $handler->display->display_options['filters']['slug']['operator'] = 'contains';
  $handler->display->display_options['filters']['slug']['exposed'] = TRUE;
  $handler->display->display_options['filters']['slug']['expose']['operator_id'] = 'slug_op';
  $handler->display->display_options['filters']['slug']['expose']['label'] = 'Slug';
  $handler->display->display_options['filters']['slug']['expose']['operator'] = 'slug_op';
  $handler->display->display_options['filters']['slug']['expose']['identifier'] = 'slug';
  $handler->display->display_options['filters']['slug']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  /* Filter criterion: PBS Show: Genre_title */
  $handler->display->display_options['filters']['genre_title']['id'] = 'genre_title';
  $handler->display->display_options['filters']['genre_title']['table'] = 'pbs_media_manager_shows';
  $handler->display->display_options['filters']['genre_title']['field'] = 'genre_title';
  $handler->display->display_options['filters']['genre_title']['exposed'] = TRUE;
  $handler->display->display_options['filters']['genre_title']['expose']['operator_id'] = 'genre_title_op';
  $handler->display->display_options['filters']['genre_title']['expose']['label'] = 'Genre';
  $handler->display->display_options['filters']['genre_title']['expose']['operator'] = 'genre_title_op';
  $handler->display->display_options['filters']['genre_title']['expose']['identifier'] = 'genre_title';
  $handler->display->display_options['filters']['genre_title']['expose']['remember_roles'] = array(
    2 => '2',
    1 => 0,
  );
  
  /* Display: Page */
  $handler = $view->new_display('page', 'Page', 'page');
  $handler->display->display_options['path'] = 'admin/content/pbs-shows';
  
  
  
  $views[$view->name] = $view;
  
  return $views;
}
