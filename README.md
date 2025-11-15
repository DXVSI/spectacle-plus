# <img src="logo.png" width="48"/> Spectacle - The KDE Screenshot Utility

Spectacle is a screenshot taking utility for the KDE desktop. Spectacle
can also be used in non-KDE X11 desktop environments.

![Screenshot of Spectacle](https://cdn.kde.org/screenshots/spectacle/spectacle.png)

## Features

### New Features (Custom Fork)

This fork adds the following features to the official Spectacle:

- **OCR (Optical Character Recognition)**: Extract text from screenshots using Tesseract OCR
  - Select a rectangular region and click the "Translate" button
  - Supports multiple languages (English, Russian, Spanish, French, German, and more)
  - Adjustable OCR language settings with automatic text scaling for better recognition

- **Translation**: Translate recognized text using MyMemory Translation API
  - Automatic language detection
  - Translate between 20+ languages
  - Remembers your language preferences
  - Interactive dialog with editable recognized text

- **FTP Upload Support**: Upload screenshots directly to FTP servers
  - Configure FTP URLs in settings (e.g., `ftp://user@server/path`)
  - Uses KDE's KIO framework for secure uploads

## Building from Source

### Dependencies

#### Required Build Tools
- CMake >= 3.16
- C++20 compatible compiler (GCC >= 11 or Clang >= 14)
- Extra CMake Modules (ECM)

#### Qt Dependencies (>= 6.9.0)
```bash
# Fedora/RHEL
sudo dnf install -y qt6-qtbase-devel qt6-qtdeclarative-devel qt6-qtmultimedia-devel \
                     qt6-qtwayland-devel qt6-qtsvg-devel

# Ubuntu/Debian
sudo apt install -y qt6-base-dev qt6-declarative-dev qt6-multimedia-dev \
                    qt6-wayland-dev libqt6svg6-dev
```

#### KDE Frameworks 6 (>= 6.18.0)
```bash
# Fedora/RHEL
sudo dnf install -y kf6-kconfig-devel kf6-kcoreaddons-devel kf6-kdbusaddons-devel \
                     kf6-ki18n-devel kf6-kio-devel kf6-knotifications-devel \
                     kf6-kwidgetsaddons-devel kf6-kxmlgui-devel kf6-kwindowsystem-devel \
                     kf6-kguiaddons-devel kf6-kglobalaccel-devel kf6-kstatusnotifieritem-devel \
                     kf6-purpose-devel

# Ubuntu/Debian
sudo apt install -y libkf6config-dev libkf6coreaddons-dev libkf6dbusaddons-dev \
                    libkf6i18n-dev libkf6kio-dev libkf6notifications-dev \
                    libkf6widgetsaddons-dev libkf6xmlgui-dev libkf6windowsystem-dev \
                    libkf6guiaddons-dev libkf6globalaccel-dev libkf6statusnotifieritem-dev \
                    libkf6purpose-dev
```

#### Additional Dependencies
```bash
# Fedora/RHEL
sudo dnf install -y opencv-devel tesseract-devel leptonica-devel \
                     wayland-protocols-devel plasma-wayland-protocols-devel \
                     layer-shell-qt-devel xcb-util-devel xcb-util-cursor-devel \
                     xcb-util-image-devel libxcb-devel

# Ubuntu/Debian
sudo apt install -y libopencv-dev libtesseract-dev libleptonica-devel \
                    wayland-protocols plasma-wayland-protocols \
                    liblayershellqtinterface-dev libxcb-util-dev libxcb-cursor-dev \
                    libxcb-image0-dev libxcb1-dev
```

### Build Instructions

```bash
# Clone the repository
git clone https://github.com/DXVSI/spectacle-.git
cd spectacle-

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake .. -DCMAKE_INSTALL_PREFIX=/usr/local

# Build (use -j$(nproc) for parallel build)
make -j32

# Install (requires sudo)
sudo make install
```

### Running

```bash
# Run from build directory
./bin/spectacle

# Or if installed
spectacle
```

## Using OCR and Translation

1. Take a screenshot or use rectangular region mode
2. Select the area containing text you want to translate
3. Click the **"Translate"** button (globe icon) on the toolbar
4. The OCR dialog will open showing:
   - **OCR Settings**: Choose the language for text recognition
   - **Recognized Text**: The extracted text (editable)
   - **Translation Settings**: Choose source and target languages
   - **Translation**: Click "Translate" to get the translation

Your language preferences are automatically saved for next time.

## Get help
You can get help in :
* Forum: https://discuss.kde.org/tag/spectacle
* Matrix: https://matrix.to/#/#kde:kde.org
* IRC: irc://irc.libera.chat/kde
## Contributing

Spectacle is developed under the KDE umbrella and uses KDE infrastructure
for development.

Please see the file [`CONTRIBUTING`](./CONTRIBUTING) for details on coding style and how
to contribute patches. Please note that pull requests on GitHub aren't
supported. The recommended way of contributing patches is via KDE's
instance of GitLab at https://invent.kde.org/plasma/spectacle.

## Release Schedule

Spectacle is released by KDE's release service and has three
major releases every year. They are numbered YY.MM, where YY is the two-
digit year and MM is the two-digit month. Major releases are made in April,
August and December every year. The Spectacle version follows the KDE
release service version.

## Reporting Bugs

Please report bugs at KDE's Bugzilla, available at https://bugs.kde.org/.

For discussions, the `#kde-devel` IRC channel and the kde-devel mailing list
are good places to post.
