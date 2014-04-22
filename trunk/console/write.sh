DRUSH="`cat .config.drush`"

echo "Writting to Example modules, please wait..."
$DRUSH -y fu example_booking

#echo "Exporting locales..."
#$DRUSH sql-dump --ordered-dump | grep -E 'INSERT INTO `(locales_source|locales_target|i18n_path|i18n_string|i18n_translation_set|l10n_update_file|l10n_update_project)`' > translations.sql

echo "Done."

