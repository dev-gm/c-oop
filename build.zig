const std = @import("std");
const fs = std.fs;

var realpath_buf: [512]u8 = undefined;

const flags = [_][]const u8{
    "-Wall",
    "-Wextra",
    "-pedantic",
    "-pedantic-errors",
    "-Werror",
    "-Iinclude/",
    "-fblocks",
    "-O3",
};

fn transverseDir(exe: *std.build.LibExeObjStep, dir: *fs.Dir, allocator: std.mem.Allocator) anyerror!void {
    var walker = try dir.walk(allocator);
    defer walker.deinit();
    while (try walker.next()) |entry| {
        switch (entry.kind) {
            .Directory => {
                var child_dir = try dir.openDir(entry.path, .{});
                defer child_dir.close();
                try transverseDir(exe, &child_dir, allocator);
            },
            .File => {
                if (std.ascii.endsWithIgnoreCase(entry.path, ".c")) {
                    exe.addCSourceFile(
                        try dir.realpath(entry.path, &realpath_buf),
                        &flags,
                    );
                }
            },
            else => {},
        }
    }
}

pub fn build(b: *std.build.Builder) void {
    // Standard target options allows the person running `zig build` to choose
    // what target to build for. Here we do not override the defaults, which
    // means any target is allowed, and the default is native. Other options
    // for restricting supported target set are available.
    const target = b.standardTargetOptions(.{});

    // Standard release options allow the person running `zig build` to select
    // between Debug, ReleaseSafe, ReleaseFast, and ReleaseSmall.
    const mode = b.standardReleaseOptions();

    var cwd_dir = fs.cwd();
    var src_dir = cwd_dir.openDir("src/", .{ .iterate = true }) catch unreachable;
    var gpa: std.heap.GeneralPurposeAllocator(.{}) = .{};
    const exe = b.addExecutable("physics-sim", null);
    transverseDir(exe, &src_dir, gpa.allocator()) catch unreachable;
    exe.linkLibC();
    exe.linkSystemLibrary("BlocksRuntime");
    exe.setTarget(target);
    exe.setBuildMode(mode);
    exe.addIncludePath("include/");
    exe.install();

    const run_cmd = exe.run();
    run_cmd.step.dependOn(b.getInstallStep());
    if (b.args) |args| {
        run_cmd.addArgs(args);
    }

    const run_step = b.step("run", "Run the app");
    run_step.dependOn(&run_cmd.step);
}
