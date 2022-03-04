{
  description = "ICU QEMU flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils, ... }:
  flake-utils.lib.eachDefaultSystem (system:
    let
      pkgs = import nixpkgs {
        inherit system;
      };
      qemu-icu = pkgs.stdenv.mkDerivation {
        name = "qemu-icu";
        src = self;
      };
    in
    rec {
      packages = flake-utils.lib.flattenTree {
        inherit qemu-icu;
      };
      defaultPackage = packages.qemu-icu;

      devShell = pkgs.mkShell {
        buildInputs = [
          pkgs.python3
          pkgs.meson
          pkgs.ninja
          pkgs.pkg-config
          pkgs.zlib
          pkgs.glib
          pkgs.perl
          pkgs.pixman
          pkgs.vde2
          pkgs.texinfo
          pkgs.lzo
          pkgs.snappy
          pkgs.libtasn1
          pkgs.gnutls
          pkgs.nettle
          pkgs.curl
          pkgs.ncurses
          pkgs.libseccomp
          pkgs.numactl
          pkgs.libaio
          pkgs.liburing
          pkgs.libxml2
          pkgs.libnfs
          pkgs.libseccomp
          pkgs.libxkbcommon
          pkgs.libiscsi

          pkgs.gdb
        ];
      };

    }
  );
}
