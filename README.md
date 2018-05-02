# Listpack Ruby

Ruby wrapper for [Redis listpack](https://gist.github.com/antirez/66ffab20190ece8a7485bd9accfbc175) data structure


## Installation

Add this line to your application's Gemfile:

```ruby
gem 'listpack'
```

And then execute:

    $ bundle

Or install it yourself as:

    $ gem install listpack

## Usage

```ruby
lp = Listpack.new

lp.append('1234')
lp.append('string')

lp.to_s
  # => "\x12\x00\x00\x00\x02\x00\xC4\xD2\x02\x86string\a\xFF"
```

#### Load a serialized listpack

```ruby
data = "\x12\x00\x00\x00\x02\x00\xC4\xD2\x02\x86string\a\xFF"

lp = Listpack.new(data)

lp.size
  # => 2

lp.next
  # => 1234
lp.next
  # => "string"
```

## Development

After checking out the repo, run `bin/setup` to install dependencies. Then, run `rake spec` to run the tests. You can also run `bin/console` for an interactive prompt that will allow you to experiment.

To install this gem onto your local machine, run `bundle exec rake install`. To release a new version, update the version number in `version.rb`, and then run `bundle exec rake release`, which will create a git tag for the version, push git commits and tags, and push the `.gem` file to [rubygems.org](https://rubygems.org).

## Contributing

Bug reports and pull requests are welcome on GitHub at https://github.com/wallin/listpack-rb.
