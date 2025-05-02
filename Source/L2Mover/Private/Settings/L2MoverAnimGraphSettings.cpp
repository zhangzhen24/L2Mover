// Fill out your copyright notice in the Description page of Project Settings.


#include "Settings/L2MoverAnimGraphSettings.h"

#include "L2MoverGameplayTags.h"

FL2MoverAnimGraphSettings::FL2MoverAnimGraphSettings()
{
	AnimLinkedLayers.FindOrAdd(L2MoverAnimLayer::Locomotion, nullptr);
	AnimLinkedLayers.FindOrAdd(L2MoverAnimLayer::Overlay, nullptr);
	AnimLinkedLayers.FindOrAdd(L2MoverAnimLayer::Extended, nullptr);
	AnimLinkedLayers.FindOrAdd(L2MoverAnimLayer::View, nullptr);
	AnimLinkedLayers.FindOrAdd(L2MoverAnimLayer::RagDoll, nullptr);
}
