Name:           ansvif
Version:        1.7
Release:        1%{?dist}
Summary:        A Not So Very Intelligent Fuzzer

License:        GPLv2
URL:            http://oxagast.github.io/ansvif
Source0:        http://github.com/oxagast/ansvif

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
cp man/ansvif.1.gz %{buildroot}/%{_mandir}/man1
%make_install


%define _unpackaged_files_terminate_build 0
%files
%license LICENSE
%{_mandir}/man1/ansvif.1.gz
%{_bindir}/ansvif
%{_bindir}/find_suid

%changelog
* Fri Nov 18 2016 Marshall Lee Whittaker
- 
