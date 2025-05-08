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
              rev = "8f6875d"; # Specify the revision
              sha256 = "XEy48IhXGLHw5+DI2oaV03P7rWzH+gSbOLYrbO2YTrE="; # Specify the SHA-256 hash
              fetchSubmodules = true; # need it for tomlpp
            };
            buildInputs = with pkgs; [
              clang
            ];
            buildPhase = ''
              				PKGS=true make -j
            '';
            installPhase = ''
              							mkdir -p /etc/webserv
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
