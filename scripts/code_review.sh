#!/bin/sh
DRUSH="drush -r ../trunk"
MODULES="example_booking"

echo "Checking code..."

set -o xtrace

$DRUSH coder-review no-empty critical comment $MODULES &
# $DRUSH coder-review no-empty critical i18n $MODULES &
$DRUSH coder-review no-empty critical security $MODULES &
$DRUSH coder-review no-empty critical sql $MODULES &
$DRUSH coder-review no-empty critical style $MODULES &

set -o xtrace -

echo $0 "done."

