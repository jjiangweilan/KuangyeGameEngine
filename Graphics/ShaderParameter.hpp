#pragma once
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>
#include "Shader.hpp"
namespace wlEngine
{
namespace Graphics
{
class ShaderParameter
{
public:
    ShaderParameter(){};
    ShaderParameter(const ShaderParameter&);
    ShaderParameter(ShaderParameter&&);
    ~ShaderParameter(){};

    ShaderParameter& operator=(const ShaderParameter&);
    ShaderParameter& operator=(ShaderParameter&&);

    template <typename T>
    void AddParameter(const std::string &name, const T &val);

    template <typename T>
    bool SetParameter(const std::string &name, const T &val);

    template <typename T>
    T *GetParameter(const std::string &name);

    void UpdateParameters(Shader* shader);

    void Use();
protected:
    class ShaderParameterBase
    {
    public:
        ShaderParameterBase(const unsigned int& loc);
        ShaderParameterBase(const ShaderParameterBase&) = default;
		
		ShaderParameterBase& operator=(const ShaderParameterBase&) = default;
		~ShaderParameterBase() {};

        virtual void Set(void *data) = 0;
        virtual void Use() = 0;
        virtual void *Get() = 0;
        virtual std::unique_ptr<ShaderParameterBase> Clone() = 0;

        void UpdateLocation(const unsigned int& loc);
    protected:
        unsigned int m_loc;
    };

    template <typename T>
    class ShaderParameterType : public ShaderParameterBase
    {
    public:
        ShaderParameterType();
        ShaderParameterType(const ShaderParameterType&) = default;
		ShaderParameterType& operator=(const ShaderParameterType&) = default;
		~ShaderParameterType();
        ShaderParameterType(const unsigned int& loc);
        void Set(const T &val);

        virtual void Set(void *data) override;
        virtual void Use() override;
        virtual void *Get() override;
        virtual std::unique_ptr<ShaderParameterBase> Clone() override;

    private:
        T m_val;
    };


    std::unordered_map<std::string, std::unique_ptr<ShaderParameterBase>> m_parameters;
};

template <typename T>
void ShaderParameter::AddParameter(const std::string &name, const T &val)
{
    m_parameters.emplace(name, new ShaderParameterType<T>(name, val));
}

template <typename T>
bool ShaderParameter::SetParameter(const std::string &name, const T &val)
{
    auto iter = m_parameters.find(name);
    if (iter == m_parameters.end())
        return false;
    iter->second->Set((void *)&val);
    return true;
}

template <typename T>
T *ShaderParameter::GetParameter(const std::string &name)
{
    auto iter = m_parameters.find(name);
    if (iter != m_parameters.end())
        return static_cast<T *>(iter->second->Get());
    return nullptr;
}

template <typename T>
ShaderParameter::ShaderParameterType<T>::ShaderParameterType() : ShaderParameterBase(""), m_val(0)
{
}

template <typename T>
ShaderParameter::ShaderParameterType<T>::~ShaderParameterType() {};

template <typename T>
ShaderParameter::ShaderParameterType<T>::ShaderParameterType(const unsigned int& loc) : ShaderParameterBase(loc), m_val()
{
}

template <typename T>
void ShaderParameter::ShaderParameterType<T>::Set(void *data)
{
    m_val = *static_cast<T *>(data);
}

template <typename T>
void ShaderParameter::ShaderParameterType<T>::Use()
{
    Shader::setUniform(m_loc, m_val);
}


template <typename T>
void *ShaderParameter::ShaderParameterType<T>::Get()
{
    return &m_val;
}
template <typename T>
void ShaderParameter::ShaderParameterType<T>::Set(const T &val)
{
    m_val = val;
}

template <typename T>
std::unique_ptr<ShaderParameter::ShaderParameterBase> ShaderParameter::ShaderParameterType<T>::Clone()
{

    return std::make_unique<ShaderParameter::ShaderParameterType<T>>(*this);
}
} // namespace Graphics
} // namespace wlEngine