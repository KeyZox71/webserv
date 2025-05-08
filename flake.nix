{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    pogit = {
      url = "github:y-syo/pogit";
      inputs.nixpkgs.follows = "nixpkgs";
    };
  };

  outputs =
    inputs@{ self, nixpkgs, ... }:
    let
      supportedSystems = [
        "x86_64-linux"
        "aarch64-linux"
        "x86_64-darwin"
        "aarch64-darwin"
      ];
      forEachSupportedSystem =
        f:
        nixpkgs.lib.genAttrs supportedSystems (
          system:
          f {
            pkgs = import nixpkgs { inherit system; };
          }
        );
    in
    {
      packages = forEachSupportedSystem (
        { pkgs }:
        rec {
          default = webserv;
          webserv = pkgs.stdenv.mkDerivation {
            name = "webserv";
            src = pkgs.fetchgit {
              url = "https://github.com/keyzox71/webserv.git";
              rev = "8ef2803"; # Specify the revision
              sha256 = "kjHPL1LInRv+8RE2Cyqt2R9M8qxpvkmvNjLhQLm3AWs="; # Specify the SHA-256 hash
              fetchSubmodules = true;
            };
            buildInputs = with pkgs; [
              clang
            ];
            buildPhase = ''
              				PKGS=true make
            '';
            installPhase = ''
              				mkdir -p $out/bin
                            	cp webserv $out/bin
            '';
          };
        }
      );
      devShells = forEachSupportedSystem (
        { pkgs }:
        {
          default = pkgs.mkShell.override { } {
            buildInputs = with pkgs; [
            ];
            hardeningDisable = [ "all" ];
            packages = with pkgs; [
              gcc11
              clang_12
              norminette
              valgrind
              git
              gdb
              inputs.pogit.packages.${pkgs.system}.default
              compiledb
            ];
          };
        }
      );
    };
}
