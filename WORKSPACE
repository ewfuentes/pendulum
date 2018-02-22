
new_http_archive(
    name = "eigen_lib",
    url = "http://bitbucket.org/eigen/eigen/get/3.3.4.tar.bz2",
    sha256 = "dd254beb0bafc695d0f62ae1a222ff85b52dbaa3a16f76e781dce22d0d20a4a6",
    strip_prefix = "eigen-eigen-5a0156e40feb",   
    build_file = "external/eigen.build"
)

bind(name = "eigen",
     actual = "@eigen_lib//:eigen"
)

new_http_archive(
    name = "gtest_lib",
    url = "https://github.com/google/googletest/archive/release-1.8.0.tar.gz",
    sha256 = "58a6f4277ca2bc8565222b3bbd58a177609e9c488e8a72649359ba51450db7d8",
	strip_prefix = "googletest-release-1.8.0",
    build_file = "external/gtest.build",
)

bind(name = "gtest",
     actual = "@gtest_lib//:main"
)
