#PBS Media Manager Migrate Module

The PBS Media Manager Migrate Module provides a mechanism to migrate COVE API Player fields to Media Manager Player fields. (Requires COVE API Player version 7.x-1.1 or higher)

##Usage
1. Add a Media Manager Player field to the entity that corresponds to the existing COVE API Player field.
2. Go to admin/config/media/pbs_media_manager/cove_migration.
3. Select the entity or content type.
4. Select the COVE API Player field that you want to migrate from.
5. Select the Media Manager Player field that you want to migrate to.
6. Click the "Migrate" button.

The module will cycle through each of the COVE API Player field instances, retrieve the Media Manager metadata from the API, and insert it into the corresponding Media Manager Player field instance.  

You will need to re-template or otherwise manage the display of the new Media Manager Player field. 