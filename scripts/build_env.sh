#!/bin/sh
DRUSH="drush -r ../trunk"
DB_URL=$(php -r 'include("../trunk/sites/default/settings.php"); extract($databases["default"]["default"]); print "$driver://$username:$password@$host/$prefix$database";')

$DRUSH site-install minimal --db-url=$DB_URL --account-pass=asdf --yes --verbose

echo "Applying release steps..."

set -o xtrace
$DRUSH -y en example_booking example_booking_feature                            # INSTALL OUR MAIN MODULES
$DRUSH -y en example_booking_structure                                           # INSTALL OUR MAIN MODULES
$DRUSH -y en menu toolbar shortcut field_ui devel_generate coder_review profile   # INSTALL CONTRIB MODULES
$DRUSH -y en diff                                                                 # INSTALL CONTRIB MODULES
$DRUSH -y vset example_booking_1st_content_type venue
$DRUSH -y vset example_booking_2nd_content_type facility
$DRUSH -y vset example_booking_3rd_content_type class
$DRUSH -y vset example_booking_4th_content_type instructors
$DRUSH -y vset error_level 1                                                      # BY DEFAULT IGNORE PHP NOTICES
set -o xtrace -

echo "Generating content..."
$DRUSH eval 'taxonomy_term_save((object) array( "name" => "Courts", "vid" => db_select("taxonomy_vocabulary", "tv")->fields("tv", array("vid"))->condition("machine_name", "type")->execute()->fetchField(), ));'
$DRUSH eval 'taxonomy_term_save((object) array( "name" => "Classes", "vid" => db_select("taxonomy_vocabulary", "tv")->fields("tv", array("vid"))->condition("machine_name", "type")->execute()->fetchField(), ));'
$DRUSH eval 'taxonomy_term_save((object) array( "name" => "45 min", "vid" => db_select("taxonomy_vocabulary", "tv")->fields("tv", array("vid"))->condition("machine_name", "duration")->execute()->fetchField(), ));'
$DRUSH eval 'taxonomy_term_save((object) array( "name" => "60 min", "vid" => db_select("taxonomy_vocabulary", "tv")->fields("tv", array("vid"))->condition("machine_name", "duration")->execute()->fetchField(), ));'
$DRUSH eval 'taxonomy_term_save((object) array( "name" => "120 min", "vid" => db_select("taxonomy_vocabulary", "tv")->fields("tv", array("vid"))->condition("machine_name", "duration")->execute()->fetchField(), ));'
$DRUSH generate-terms classes_types 20
$DRUSH generate-terms sports_categories 20
$DRUSH generate-terms sports_categories 20
$DRUSH generate-content --types=instructors 50
$DRUSH generate-content --types=class 50
$DRUSH generate-content --types=venue 20
$DRUSH generate-content --types=facility 20

echo "Admin access: admin/asdf or please access using the following link:"
$DRUSH -y uli                                                                     # GENERATE ONE-TIME LOGIN LINK FOR ADMIN

echo $0 "done."

