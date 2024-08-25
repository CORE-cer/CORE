{
  description = "A basic flake with a shell";
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixos-24.05";
  inputs.flake-utils.url = "github:numtide/flake-utils";

  outputs =
    { nixpkgs, flake-utils, ... }:
    flake-utils.lib.eachDefaultSystem (
      system:
      let
        pkgs = nixpkgs.legacyPackages.${system};
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
              clang_18
              gcc14
            ]
            ++ (if pkgs.stdenv.isDarwin then [ ] else [ valgrind ]);
        };
      }
    );
}
