Office Hours (Drupal 7 version) now supports Feeds module to import data.

The following columns can be used: day; hours/morehours from; hours/morehours to; hours/morehours from-to.

The day should be stated in full English name, or a day number where sunday = 0, monday=1, etc.
The hours can be formatted as hh:mm or hh.mm

I suppose Feeds Tamper can help to format the times and/or day to the proper format.

Here's an example :
nid;weekday;Hours_1;Hours_2
2345;monday;11:00 - 18:01;
2345;tuesday;10:00 - 12:00;13:15-17.45
2383;monday;11:00 - 18:01;
2383;tuesday;10:00 - 12:00;13:15-17.45

Also, the D7 version has the possibility to format the office hours any way you want, using the customizable separators. E.g. the following example for Google Places-format, for a shop that opens from Monday to Friday: 
  2:10:00:18:00,3:10:00:18:00,4:10:00:18:00,5:10:00:18:00,6:10:00:18:00,7:12:00:20:00
