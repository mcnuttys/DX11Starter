#include "Material.h"

Material::Material(DirectX::XMFLOAT3 colorTint, float roughness, std::shared_ptr<SimpleVertexShader> vertexShader, std::shared_ptr<SimplePixelShader> pixelShader)
{
    SetColorTint(colorTint);
    SetRoughness(roughness);
    SetVertexShader(vertexShader);
    SetPixelShader(pixelShader);
}

DirectX::XMFLOAT3 Material::GetColorTint()
{
    return colorTint;
}

float Material::GetRoughness()
{
    return roughness;
}

std::shared_ptr<SimpleVertexShader> Material::GetVertexShader()
{
    return vertexShader;
}

std::shared_ptr<SimplePixelShader> Material::GetPixelShader()
{
    return pixelShader;
}

void Material::SetColorTint(DirectX::XMFLOAT3 _colorTint)
{
    colorTint = _colorTint;
}

void Material::SetRoughness(float _roughness)
{
    roughness = _roughness;
}

void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> _vertexShader)
{
    vertexShader = _vertexShader;
}

void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> _pixelShader)
{
    pixelShader = _pixelShader;
}
