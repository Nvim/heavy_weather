{
  description = "Some graphics engine again";

  inputs = { };

  outputs =
    {
      nixpkgs,
      ...
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };
      lib = pkgs.lib;
    in
    {
      devShells.${system}.default = pkgs.mkShell.override { stdenv = pkgs.clang18Stdenv; } {
        # LD_LIBRARY_PATH = "${lib.getLib pkgs.libGL}/lib/libGL.so.1:${lib.getLib pkgs.libGL}/lib/libEGL.so.1";
        LD_LIBRARY_PATH =
          let
            libPath = lib.makeLibraryPath ([
              pkgs.libglvnd
              pkgs.libxkbcommon
              pkgs.xorg.libXcursor
              pkgs.xorg.libXext
              pkgs.xorg.libXrandr
              pkgs.xorg.libXi
              pkgs.wayland
            ]);
          in
          "${libPath}";

        buildInputs = with pkgs; [
          # Vendor building dependencies:
          # GLFW:
          libGL
          libglvnd
        ];
        packages = with pkgs; [
          # IDE/Dev Tools:
          cmakeWithGui
          pkg-config
          ninja
          clang-tools_18 # LSP & Formatter
          lldb_18 # Debugger
          pre-commit
          gdbgui

          # Vendor (building them along project is cooler):
          glfw-wayland
          glm

          # runtime for imgui:
          libGL
          libglvnd
        ];
      };
    };
}
