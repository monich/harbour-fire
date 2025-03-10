Name:           harbour-fire

Summary:        Fireplace
Version:        1.0.5
Release:        1
License:        BSD
URL:            https://github.com/monich/harbour-fire
Source0:        %{name}-%{version}.tar.gz

Requires:       sailfishsilica-qt5
Requires:       qt5-qtsvg-plugin-imageformat-svg
BuildRequires:  pkgconfig(sailfishapp)
BuildRequires:  pkgconfig(Qt5Core)
BuildRequires:  pkgconfig(Qt5DBus)
BuildRequires:  pkgconfig(Qt5Qml)
BuildRequires:  pkgconfig(Qt5Quick)

%{!?qtc_qmake5:%define qtc_qmake5 %qmake5}
%{!?qtc_make:%define qtc_make make}
%{?qtc_builddir:%define _builddir %qtc_builddir}

%description
Light your fire

%if "%{?vendor}" == "chum"
Categories:
 - Other
Icon: https://raw.githubusercontent.com/monich/harbour-fire/master/icons/harbour-fire.svg
Screenshots:
- https://home.monich.net/chum/harbour-fire/screenshots/screenshot-001.png
- https://home.monich.net/chum/harbour-fire/screenshots/screenshot-002.png
Url:
  Homepage: https://openrepos.net/content/slava/fire
%endif

%prep
%setup -q -n %{name}-%{version}

%build
%qtc_qmake5 %{name}.pro
%qtc_make %{?_smp_mflags}

%install
rm -rf %{buildroot}
%qmake5_install

desktop-file-install --delete-original \
  --dir %{buildroot}%{_datadir}/applications \
   %{buildroot}%{_datadir}/applications/*.desktop

%files
%defattr(-,root,root,-)
%{_bindir}/%{name}
%{_datadir}/%{name}
%{_datadir}/applications/%{name}.desktop
%{_datadir}/icons/hicolor/*/apps/%{name}.png
