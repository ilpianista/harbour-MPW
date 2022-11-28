Name:       libmpw

# >> macros
# << macros

Summary:    MasterPassword library
Version:    2.6
Release:    3
Group:      System/Libraries
License:    GPLv3
URL:        https://masterpassword.app/
Source0:    mpw-2.6-cli-5-0-g344771db.tar.gz
Patch0:     fix-build.patch
Requires:   glibc >= 2.15
Requires:   openssl-libs
Requires:   json-c
BuildRequires:  glibc-devel >= 2.15
BuildRequires:  openssl-devel
BuildRequires:  pkgconfig(openssl)
BuildRequires:  pkgconfig(ncurses)
BuildRequires:  pkgconfig(json-c)
BuildRequires:  pkgconfig(libsodium)

%description
The MasterPassword library

%package devel
Summary: MasterPassword headers

%description devel
The MasterPassword headers

%prep
%setup -q -c
%patch0 -p1

# >> setup
# << setup

%build
# >> build pre
export CFLAGS="-fPIC -DMPW_SODIUM=1"
targets='mpw' ./build -lrt
# << build pre

# >> build post
gcc -std=gnu99 -Ilib/include -Icore -Icli -shared $CFLAGS -L. -o libmpw.so core/mpw-algorithm.c core/mpw-types.c core/mpw-util.c core/base64.c core/aes.c
# << build post

%install
rm -rf %{buildroot}
# >> install pre
# << install pre

mkdir -p %{buildroot}%{_libdir}
install -Dm644 libmpw.so %{buildroot}%{_libdir}/libmpw.so.0.0
ln -sf %{_libdir}/libmpw.so.0.0 %{buildroot}%{_libdir}/libmpw.so.0
ln -sf %{_libdir}/libmpw.so.0.0 %{buildroot}%{_libdir}/libmpw.so

mkdir -p %{buildroot}/usr/include/mpw
install -Dm644 core/mpw-{algorithm,types,util}.h %{buildroot}/usr/include/mpw
# >> install post
# << install post

%post -n libmpw -p /sbin/ldconfig

%postun -n libmpw -p /sbin/ldconfig

%clean
rm -rf %{buildroot}

%files
%defattr(-,root,root,-)
%{_libdir}/libmpw.so*
# >> files
# << files

%files devel
%defattr(-,root,root,-)
%{_includedir}/mpw/*.h
# >> files
# << files
