<div class="pbs_media_manager-program-block">
  <?php if ($variables['pbs_media_manager_program']['make_link']) { ?>
  <a href="<?php print $variables['pbs_media_manager_program']['link']?>" ><?php }?>
  <?php if ($variables['pbs_media_manager_program']['image_url'] != ''){?>
  <img src="<?php print $variables['pbs_media_manager_program']['image_url']?>" >
  <?php } ?>
  <?php if ($variables['pbs_media_manager_program']['title'] != ''){?>
  <h3 class='pbs_media_manager-program-title'><?php print $variables['pbs_media_manager_program']['title']?></h3>
  <?php } ?>
  <?php if ($variables['pbs_media_manager_program']['make_link']) { ?></a><?php } ?>
  <?php if ($variables['pbs_media_manager_program']['description'] != ''){?>
  <p class="pbs_media_manager-program-desc"><?php print $variables['pbs_media_manager_program']['description']?></p>
  <?php } ?>

</div>
