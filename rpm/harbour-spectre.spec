Name:       harbour-spectre

# >> macros
%define __provides_exclude_from ^%{_datadir}/.*$
%define __requires_exclude ^libsodium.*|libspectre.*$
# << macros

%{!?qtc_qmake:%define qtc_qmake %qmake}
%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}
Summary:    A Spectre client
Version:    0.4.0
Release:    1
Group:      Qt/Qt
License:    MIT
URL:        https://scarpino.dev
Source0:    %{name}-%{version}.tar.bz2
Requires:   sailfishsilica-qt5 >= 0.10.9
BuildRequires:  pkgconfig(sailfishapp) >= 1.0.2
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)
BuildRequires:  pkgconfig(openssl)
BuildRequires:  desktop-file-utils
%if "%{?vendor}" == "chum"
Obsoletes:  harbour-mpw
Obsoletes:  libmpw
Obsoletes:  libmpw-devel
%endif

%description
A Spectre client.

%if "%{?vendor}" == "chum"
PackageName: Spectre
Type: desktop-application
DeveloperName: Andrea Scarpino
Categories:
 - Utility
Custom:
  Repo: https://github.com/ilpianista/harbour-Spectre
Icon: https://raw.githubusercontent.com/ilpianista/harbour-Spectre/master/icons/harbour-spectre.svg
Screenshots:
 - https://raw.githubusercontent.com/ilpianista/harbour-Spectre/master/screenshots/screenshot_1.png
Url:
  Homepage: https://github.com/ilpianista/harbour-Spectre
  Bugtracker: https://github.com/ilpianista/harbour-Spectre/issues
  Donation: https://liberapay.com/ilpianista
%endif


%prep
%setup -q -n %{name}-%{version}

# >> setup
# << setup

%build
# >> build pre
# << build pre

%qtc_qmake5

%qtc_make %{?_smp_mflags}

# >> build post
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre
%qmake5_install

# >> install post
# << install post

desktop-file-install --delete-original       \
  --dir %{buildroot}%{_datadir}/applications             \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
# >> files
# << files
