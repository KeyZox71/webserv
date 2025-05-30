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
              rev = "b418e4c"; # Specify the revision
              sha256 = "EtYxNbxtsmEVpx3HN31HUoUbUrLqI++6mXqBP8CAM2w="; # Specify the SHA-256 hash
              fetchSubmodules = true; # need it for tomlpp
            };
            buildInputs = with pkgs; [
              clang_12
            ];
            buildPhase = ''
              				PKGS=true make -j
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
          default = pkgs.mkShell {
            hardeningDisable = [ "all" ];
            nativeBuildInputs = with pkgs; [
              llvmPackages_12.clang-tools
              gcc11
              clang_12
            ];
            buildInputs = with pkgs; [
            ];
            packages = with pkgs; [
              nixd
              nixfmt-rfc-style
              norminette
              siege
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
