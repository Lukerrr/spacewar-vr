// Copyright (C) 2022 GB ltd. All rights reserved.

#include "UMG/Components/MultimaterialWidget.h"

UMaterialInterface* UMultimaterialWidget::GetMaterial(int32 MaterialIndex) const
{
	auto Result = Super::GetMaterial(MaterialIndex);

#define OVERRIDE_MATERIAL(_old, _new) if (Result == _old) return _new ? _new : _old;

	OVERRIDE_MATERIAL(TranslucentMaterial, TranslucentMaterial_Override);
	OVERRIDE_MATERIAL(TranslucentMaterial_OneSided, TranslucentMaterial_OneSided_Override);
	OVERRIDE_MATERIAL(OpaqueMaterial, OpaqueMaterial_Override);
	OVERRIDE_MATERIAL(OpaqueMaterial_OneSided, OpaqueMaterial_OneSided_Override);
	OVERRIDE_MATERIAL(MaskedMaterial, MaskedMaterial_Override);
	OVERRIDE_MATERIAL(MaskedMaterial_OneSided, MaskedMaterial_OneSided_Override);

#undef OVERRIDE_MATERIAL

	return Result;
}
