# frozen_string_literal: true

RSpec.describe Listpack do
  describe '::dump' do
    subject { described_class.dump(input) }

    context 'when input is ["1", "2"]' do
      let(:input) { ['1', '2'] }

      it { is_expected.to eq "\v\x00\x00\x00\x02\x00\x01\x01\x02\x01\xFF".b }
    end

    context 'when input is [1, 2]' do
      let(:input) { [1, 2] }

      it { is_expected.to eq "\v\x00\x00\x00\x02\x00\x01\x01\x02\x01\xFF".b }
    end
  end

  describe '::load' do
    subject { described_class.load(input) }

    context 'when input is [1, 2] encoded' do
      let(:input) { "\v\x00\x00\x00\x02\x00\x01\x01\x02\x01\xFF".b }

      it { is_expected.to eq [1, 2] }
    end
  end
end
