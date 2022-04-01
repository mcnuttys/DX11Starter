#include "Material.h"

Material::Material(
    DirectX::XMFLOAT3 colorTint,
    float roughness,
    DirectX::XMFLOAT2 scale,
    DirectX::XMFLOAT2 offset,
    std::shared_ptr<SimpleVertexShader> vertexShader,
    std::shared_ptr<SimplePixelShader> pixelShader
)
{
    SetColorTint(colorTint);
    SetRoughness(roughness);
    SetUVScale(scale);
    SetUVOffset(offset);
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

DirectX::XMFLOAT2 Material::GetUVScale()
{
    return scale;
}

DirectX::XMFLOAT2 Material::GetUVOffset()
{
    return offset;
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

void Material::SetUVScale(DirectX::XMFLOAT2 _scale)
{
    scale = _scale;
}

void Material::SetUVOffset(DirectX::XMFLOAT2 _offset)
{
    offset = _offset;
}

void Material::SetVertexShader(std::shared_ptr<SimpleVertexShader> _vertexShader)
{
    vertexShader = _vertexShader;
}

void Material::SetPixelShader(std::shared_ptr<SimplePixelShader> _pixelShader)
{
    pixelShader = _pixelShader;
}

void Material::AddTextureSRV(std::string name, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv)
{
    textureSRVs.insert({ name, srv });

}

void Material::AddSampler(std::string name, Microsoft::WRL::ComPtr<ID3D11SamplerState> sampler)
{
    samplers.insert({ name, sampler });
}

void Material::PrepareMaterial(Transform* transform, Camera* camera)
{
    vertexShader->SetShader();
    pixelShader->SetShader();

    vertexShader->SetMatrix4x4("world", transform->GetWorldMatrix());
    vertexShader->SetMatrix4x4("worldInvTranspose", transform->GetWorldInverseTransposeMatrix());
    vertexShader->SetMatrix4x4("view", camera->GetViewMatrix());
    vertexShader->SetMatrix4x4("projection", camera->GetProjectionMatrix());
    vertexShader->CopyAllBufferData();

    pixelShader->SetShader();
    pixelShader->SetFloat3("colorTint", GetColorTint());
    pixelShader->SetFloat("roughness", GetRoughness());
    pixelShader->SetFloat2("UVScale", GetUVScale());
    pixelShader->SetFloat2("UVOffset", GetUVOffset());
    pixelShader->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
    pixelShader->CopyAllBufferData();

    for (auto& t : textureSRVs) { pixelShader->SetShaderResourceView(t.first.c_str(), t.second.Get()); }
    for (auto& s : samplers) { pixelShader->SetSamplerState(s.first.c_str(), s.second.Get()); }
}
