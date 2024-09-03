{
  description = "A basic flake with a shell";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs =
    {
      nixpkgs,
      flake-utils,
      ...
    }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
        is_dev = builtins.pathExists ./is-dev;
      in
      {
        devShells.default = pkgs.mkShell {
          packages =
            with pkgs;
            [
              bashInteractive
              zsh
              clang-tools
              parallel
              conan
              cmake
              ninja
            ]
            ++ nixpkgs.lib.optionals (!is_dev) [

              clang_18
              valgrind
              gcc14
            ];
          LOCALE_ARCHIVE = "${pkgs.glibcLocales}/lib/locale/locale-archive";
        };
      }
    );
}
