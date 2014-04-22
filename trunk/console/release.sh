DRUSH="`cat .config.drush`"

echo "Releasing Example, please wait..."

echo "1/4: Clearing database..."
$DRUSH -y sql-drop

echo "2/4: Importing clean database... (may take 30s)"
$DRUSH sql-cli < ./db.sql || mysql -fu `cat .config.db.user` -p`cat .config.db.pass` `cat .config.db.name` < ./db.sql

echo "3/4: Enabling Example module(s) and all its dependencies..."
$DRUSH -y dis overlay
$DRUSH -y en profile example_booking

#echo "Updating locales..."
#$DRUSH -y l10n-update-refresh
#$DRUSH -y l10n-update

echo "4/4: Finishing..."
$DRUSH -y fr example_booking

$DRUSH -y uli
