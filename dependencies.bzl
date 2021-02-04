load("@rules_third_party//libraries:lua.bzl", "lua")
load("@rules_foreign_cc//:workspace_definitions.bzl", "rules_foreign_cc_dependencies")

def dependencies():
    rules_foreign_cc_dependencies()
    lua()
