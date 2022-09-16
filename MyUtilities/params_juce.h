class _param : public juce::AudioProcessorParameter
{
public:
    float default_val{ 0 };
    float current_val{ 0 };
    juce::String name;

    float getValue() const override
    {
        return current_val;
    }

    void setValue(float newValue) override
    {
        current_val = newValue;
    }

    float getDefaultValue() const override
    {
        return default_val;
    }

    juce::String getName(int maximumStringLength) const override
    {
        return name;
    }

    juce::String getLabel() const override
    {
        return getName(10);
    }

    float getValueForText(const juce::String& text) const override
    {
        return 1;
    }
};