set -e

PLUGIN_NAME=wolf-spectrum
PLUGIN_VERSION=$(git describe --tags --abbrev=0)
TARBALL_NAME="$PLUGIN_NAME-$PLUGIN_VERSION-source.tar.gz"

rm -rf "$PLUGIN_NAME"
git clone --recursive "https://github.com/pdesaulniers/$PLUGIN_NAME.git" --branch "$PLUGIN_VERSION" --single-branch
cd "$PLUGIN_NAME"
find . -name ".git" -type f -delete
rm -rf .git
find . -name ".gitmodules" -type f -delete
cd ..
tar -zcvf "./.releases/$TARBALL_NAME" "$PLUGIN_NAME/"
rm -rf "$PLUGIN_NAME/"

#gpg --armor --detach-sign "$TARBALL_NAME"
