# frozen_string_literal: true

lib = File.expand_path('lib', __dir__)
$LOAD_PATH.unshift(lib) unless $LOAD_PATH.include?(lib)

require "listpack/version"

Gem::Specification.new do |spec|
  spec.name          = "listpack"
  spec.version       = Listpack::VERSION
  spec.authors       = ["Sebastian Wallin"]
  spec.email         = ["sebastian.wallin@gmail.com"]
  spec.extensions    = ["ext/listpack/extconf.rb"]

  spec.summary       = %q{Ruby wrapper for Redis listpack data structure}
  spec.description   = %q{Ruby wrapper for Redis listpack data structure}
  spec.homepage      = "https://github.com/wallin/listpack-rb"
  spec.license       = 'BSD-3-Clause'

  spec.files         = `git ls-files -z`.split("\x0").reject do |f|
    f.match(%r{^(test|spec|features)/})
  end
  spec.bindir        = "exe"
  spec.executables   = spec.files.grep(%r{^exe/}) { |f| File.basename(f) }
  spec.require_paths = ["lib"]

  spec.add_dependency "rake-compiler"
end
