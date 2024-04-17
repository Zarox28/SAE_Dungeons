const std = @import("std");
const raylib = @import("libs/raylib/src/build.zig");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});

    const optimize = b.standardOptimizeOption(.{});

    const exe = b.addExecutable(.{
        .name = "dungeon",
        .target = target,
        .optimize = optimize,
    });

    const lib = raylib.addRaylib(b, target, optimize, .{ .linux_display_backend = .Wayland }) catch @panic("Failed to build raylib !");

    exe.linkLibCpp();
    exe.linkLibrary(lib);
    exe.addIncludePath(std.Build.LazyPath.relative("libs/raylib/src"));
    exe.addIncludePath(std.Build.LazyPath.relative("include"));
    exe.addCSourceFiles(.{
        .files = &.{
            "main.cpp",
            "src/dungeon/dungeon.cpp",
            "src/graph/graph.cpp",
        },
    });

    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);

    run_cmd.step.dependOn(b.getInstallStep());

    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the executable");

    run_step.dependOn(&run_cmd.step);
}
