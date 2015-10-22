Booking Example Site
====================

This site provides demo for testing [Booking Time Slots module](https://drupal.org/project/booking_timeslots).

Alternatively you may consider this [sandbox](https://simplytest.me/project/booking_timeslots/7.x-1.x).

Requirements
------------

 - Unix-like command-line access
 - configured LAMP environment
 - drush
 
Installation
------------

1. Check database credentials in trunk/sites/default/settings.php (you might need to copy and rename default.settings.php into settings.php)
2. Install environment by the following command from webroot:

        cd ./scripts && ./build_env.sh

3. Access your site by pointing your HTTP server to site's webroot trunk directory, e.g. http://booking.example/

### Windows

Here are detailed step based on this [post](https://www.drupal.org/node/2401547#comment-10477340):

1. Download the demo site and place it in your WWW folder (for Ubuntu or WAMP) or in htdocs folder (for XAMPP and windows).
1. Rename the folder of demo site whatever you want. In my case, it was timeslot
1. Now, create a new database in phpmyadmin with name of timesolt (or whatever you want) and import backup file from your root folder. In my case, its path was timeslot/trunk/console/db.sql. Note: Name of my root folder is timeslot.
1. After you've imported your backup into newly created database, make a virtual host for demo site that you've downloaded. Note: This step is not compulsory.
1. Then you need to go to timeslot/trunk/sites/default folder and copy default.settings.php into same folder and rename it as settings.php
1. Then go to path of your virtulahost that you created or simply go to localhost/timeslot/trunk to open demo site in browser. It will ask usual drupal setup steps. Provide the name of databse in which you imported backup of demo site. 1. Site will be installed and get working.
1. Then in your command prompt, go to root folder (in my case it was timeslot) and run the following command.

        cd ./scripts && ./build_env.sh

1. Drush will do the rest and compulsory modules would be setup in your site and will be ready to be explored.

Demo
----
1. Log-in as admin (admin/admin).
2. Go to: Administration » Configuration » Example Booking (/admin/config/example_booking/settings) and re-save the settings.
3. Open any of the venues and go to the "Opening Hours" tab.
4. Click on some day of week to add venue opening hours.
5. Enter start time e.g. 08:00 and end time e.g. 20:00. It will restrict facilities' and classes' opening hours to be lay between specified hours.
6. Open any of the facilities and go to "Edit" tab.
7. Specify previously edited venue as the "Venue Facility belongs to". You may also specifiy categories(sports) of the facility. You may later filter facilities by that category.
8. Go to the "Opening Hours" tab of the facility.
9. Click on the day when previously edited venue is open and specify facility opening hours (must lay between venue opening hours), pricing (a must) and other things. Please use non regular hours for testing.
10. You may finally open the venue and go to "Schedule" tab to display calendar and be able to book slots.
   Note: If you don't see the booking, make sure that you've selected the right filters (e.g. the right sport category is selected which is provided by that Facility).
11. If you create class, you may relate it with the facility. This will allow you to create bookable slots of custom length (user must book the whole slot instead of selecting custom length).
12. If you create instructor, you may relate it with the class to enable filtering classes by instructor (you won't be able to create slots per instructor so 4th level content type acts like classes filter).

Those steps should allow you to book timeslots.
