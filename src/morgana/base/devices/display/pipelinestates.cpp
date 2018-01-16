#include "pipelinestates.h"

using namespace MorganaEngine::Base::Devices::Display;

__implement_class_abstract(PipelineState, MEObject);

__implement_class(BlendState, PipelineState);
__implement_class(DepthState, PipelineState);
__implement_class(StencilState, PipelineState);
__implement_class(RasterizerState, PipelineState);

const BlendState *BlendState::Additive = new BlendState(PipelineState::BlendFactor::One, PipelineState::BlendFactor::One);
const BlendState *BlendState::Multiply = new BlendState(PipelineState::BlendFactor::DstColor, PipelineState::BlendFactor::Zero);
const BlendState *BlendState::AlphaBlend = new BlendState(PipelineState::BlendFactor::SrcAlpha, PipelineState::BlendFactor::OneMinusSrcAlpha);
const BlendState *BlendState::Opaque = new BlendState(PipelineState::BlendFactor::One, PipelineState::BlendFactor::Zero);

const DepthState *DepthState::LessEqual = new DepthState(true, true, PipelineState::CompareFunc::LessEqual);
const DepthState *DepthState::LessEqualNoWrite = new DepthState(true, false, PipelineState::CompareFunc::LessEqual);
const DepthState *DepthState::Equal = new DepthState(true, false, PipelineState::CompareFunc::Equal);
const DepthState *DepthState::Disabled = new DepthState(false, false, PipelineState::CompareFunc::Always);

const StencilState* StencilState::Disabled = new StencilState(false);

const RasterizerState	*RasterizerState::CullNone = new RasterizerState(PipelineState::RasterCullOp::None);
const RasterizerState	*RasterizerState::CullFront = new RasterizerState(PipelineState::RasterCullOp::Front);
const RasterizerState	*RasterizerState::CullBack = new RasterizerState(PipelineState::RasterCullOp::Back);