Name:           ansvif
Version:        1.9.1
Release:        1%{?dist}
Summary:        A Not So Very Intelligent Fuzzer

License:        GPLv2
URL:            http://oxagast.github.io/ansvif
Source0:        %{name}-%{version}.tar.gz

BuildRequires:  gcc automake autoconf zlib-devel cryptopp-devel
Requires:       gcc

%description
A Not So Very Intelligent Fuzzer: An advanced cross platform featureful fuzzing framework designed to find vulnerabilities in C/C++ code.

%prep
%autosetup


%build
aclocal && autoconf && automake -a
%configure
%make_build

%install
rm -rf $RPM_BUILD_ROOT
mkdir -p %{buildroot}/%{_mandir}/man1
cp man/*gz %{buildroot}/%{_mandir}/man1
mkdir -p %{buildroot}/%{_bindir}
cp tools/find_suid.sh %{buildroot}/usr/bin/find_suid
mkdir -p %{buildroot}/usr/share/ansvif/examples
cp examples/all examples/specific examples/biglist %{buildroot}/usr/share/ansvif/examples -r
%make_install


%define _unpackaged_files_terminate_build 0
%files
%license LICENSE
%{_mandir}/man1/ansvif.1.gz
%{_mandir}/man1/find_suid.1.gz
%{_bindir}/ansvif
%{_bindir}/find_suid
%{_bindir}/ansvif_gtk
/usr/share/ansvif/examples/biglist
/usr/share/ansvif/examples/all
/usr/share/ansvif/examples/specific/

%changelog
* Tue Apr 17 2018 Marshall Lee Whittaker
- 
