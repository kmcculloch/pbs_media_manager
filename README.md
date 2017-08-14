@codingStandardsIgnoreFile
#PBS Media Manager Module

The Media Manager API allows stations, producers, and viewers of PBS.org search and view digital assets. This module is not officially affiliated with, or maintained by, PBS.

Use of this module requires an API key, which can be [requested from PBS](http://digitalsupport.pbs.org/support/tickets/new). Additional information about the Media Manager API can be found in the [Developer Documentation](https://docs.pbs.org/display/CDA/Media+Manager+API).

Included submodules:

**PBS Media Manager API Test Interface:** Allows admins to run queries against the API to see what comes back.

**PBS Media Manager Show:** Provides Media Manager shows as Drupal entities. (Requires Views and Entity API)

**PBS Media Manager Player:** Provides a field to embed PBS partner players through the Media Manager API.

**PBS Media Manager Migrate:** Provides a mechanism to migrate COVE API Player fields to Media Manager Player fields. (Requires COVE API Player version 7.x-1.1 or higher)


##Installation

Install as any Drupal module. Place the module in the appropriate modules directory (typically sites/all/modules/contrib), and enable through drush or the Drupal interface.

##Credits

The main module development by [Aaron Crosman](https://www.drupal.org/u/acrosman) at Cyberwoven on behalf of SCETV, and [Jess Snyder](https://www.drupal.org/u/jesss) at WETA.

The client library is from Will Tam at WNET: https://github.com/tamw-wnet/PBS_Media_Manager_Client
