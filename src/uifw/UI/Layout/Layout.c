#include "Layout.h"

#include "uifw/Core/Containers/Queue.h"
#include "uifw/Core/Utils/Log.h"
#include "uifw/ECS/Components.h"

static int32_t clamp_value_signed(const int32_t value,
                                  const int32_t min,
                                  const int32_t max)
{
  if (value < min) {
    return min;
  }

  if (value > max) {
    return max;
  }

  return value;
}

static void layout_children(const ui_ECS_Entity *entity,
                            ui_ECS_World *world,
                            ui_Queue *queue,
                            uint16_t nIteration)
{
  ECS_COMPONENT(world, ui_ECS_BaseComponent);
  ECS_COMPONENT(world, ui_ECS_TransformRelComponent);
  ECS_COMPONENT(world, ui_ECS_LayoutComponent);

  if (!ecs_has(world, *entity, ui_ECS_BaseComponent)) {
    return;
  }

  const auto parentBaseComponent = ecs_get(world, *entity, ui_ECS_BaseComponent);
  const auto parentTransformRel = ecs_get(world, *entity, ui_ECS_TransformRelComponent);
  const auto parentLayoutComponent = ecs_get(world, *entity, ui_ECS_LayoutComponent);

  const size_t nChildren = parentTransformRel->nChildren;

  if (nChildren < 1) {
    return;
  }

  const ui_LayoutType layoutType = parentLayoutComponent->type;
  const ui_LayoutPadding margins = parentLayoutComponent->margins;
  const uint16_t spacing = parentLayoutComponent->spacing;

  ui_ECS_BaseComponent *children[nChildren];

  ui_ECS_Entity currentEntity = parentTransformRel->first;
  auto currentRel =
    ecs_get_mut(world, currentEntity, ui_ECS_TransformRelComponent);

  for (size_t i = 0; i < nChildren; ++i) {
    children[i] = ecs_get_mut(world, currentEntity, ui_ECS_BaseComponent);

    if (ecs_has(world, *entity, ui_ECS_LayoutComponent)) {
      ui_queuePush(queue, &currentEntity);
    }

    if (currentRel->next == UI_NULL_ENTITY) {
      break;
    }

    currentEntity = currentRel->next;
    currentRel = ecs_get_mut(world, currentEntity, ui_ECS_TransformRelComponent);
  }

  // Find min/max constraints for current layout axis
  uint16_t minConstraints[nChildren];
  uint16_t maxConstraints[nChildren];

  for (size_t i = 0; i < nChildren; ++i) {
    switch (layoutType) {
    case LayoutType_Horizontal:
      minConstraints[i] = children[i]->minWidth;
      maxConstraints[i] = children[i]->maxWidth;
      break;
    case LayoutType_Vertical:
      minConstraints[i] = children[i]->minHeight;
      maxConstraints[i] = children[i]->maxHeight;
      break;
    }
  }

  // Fit components to available space
  int32_t availableSpace = 0;
  int32_t currentPosition = 0;

  switch (layoutType) {
  case LayoutType_Horizontal:
    availableSpace = (int32_t)parentBaseComponent->coords.w - (int32_t)margins.left -
      (int32_t)margins.right - (int32_t)spacing * (int32_t)(nChildren - 1);
    currentPosition = (int32_t)parentBaseComponent->coords.x + (int32_t)margins.left;
    break;
  case LayoutType_Vertical:
    availableSpace = (int32_t)parentBaseComponent->coords.h - (int32_t)margins.top -
      (int32_t)margins.bottom - (int32_t)spacing * (int32_t)(nChildren - 1);
    currentPosition = (int32_t)parentBaseComponent->coords.y + (int32_t)margins.top;
    break;
  default:
    break;
  }

  // Clamp availableSpace to prevent underflow artifacts
  availableSpace = clamp_value_signed(availableSpace, 0, UINT16_MAX);

  const int32_t initialSpace = availableSpace;
  int32_t computedSizes[nChildren];
  size_t nRemainingComponents = nChildren;

  // Calculate fixed-sized components first
  for (size_t i = 0; i < nChildren; ++i) {
    const uint16_t minSize = minConstraints[i];
    const uint16_t maxSize = maxConstraints[i];

    if (minSize == 0 && maxSize == UINT16_MAX) {
      // Min/max size not set, skip entity in this pass
      continue;
    }

    // Fixed size: use minSize as the target (min and max are set to the same value)
    int32_t fixedSize = (int32_t)minSize;
    fixedSize = clamp_value_signed(fixedSize, (int32_t)minSize, (int32_t)maxSize);

    // Ensure fixed size doesn't exceed available space
    fixedSize = clamp_value_signed(fixedSize, 0, availableSpace);

    computedSizes[i] = fixedSize;
    availableSpace -= fixedSize;

    nRemainingComponents--;
  }

  // Then calculate flexible entities
  for (size_t i = 0; i < nChildren; ++i) {
    const uint16_t minSize = minConstraints[i];
    const uint16_t maxSize = maxConstraints[i];

    if (minSize > 0 || maxSize < UINT16_MAX) {
      // Min/max size value specified, already handled in first pass
      continue;
    }

    if (nRemainingComponents == 0) {
      computedSizes[i] = 0;
      continue;
    }

    int32_t inferredSize = availableSpace / (int32_t)nRemainingComponents;
    inferredSize = clamp_value_signed(inferredSize, (int32_t)minSize, (int32_t)maxSize);

    computedSizes[i] = inferredSize;
    availableSpace -= inferredSize;

    nRemainingComponents--;
  }

  // Calculate secondary axis
  int32_t secondaryAxisSize = 0;
  int32_t secondaryAxisPosition = 0;

  switch (layoutType) {
  case LayoutType_Horizontal:
    secondaryAxisSize = (int32_t)parentBaseComponent->coords.h - (int32_t)margins.top -
      (int32_t)margins.bottom;
    secondaryAxisPosition = (int32_t)parentBaseComponent->coords.y + (int32_t)margins.top;
    break;
  case LayoutType_Vertical:
    secondaryAxisSize = (int32_t)parentBaseComponent->coords.w - (int32_t)margins.left -
      (int32_t)margins.right;
    secondaryAxisPosition = (int32_t)parentBaseComponent->coords.x + (int32_t)margins.left;
    break;
  }

  // Clamp secondary axis to prevent underflow
  secondaryAxisSize = clamp_value_signed(secondaryAxisSize, 0, UINT16_MAX);

  // Set sizes
  for (size_t i = 0; i < nChildren; ++i) {
    switch (layoutType) {
    case LayoutType_Horizontal:
      children[i]->coords.x = (uint16_t)clamp_value_signed(currentPosition, 0, UINT16_MAX);
      children[i]->coords.y = (uint16_t)clamp_value_signed(secondaryAxisPosition, 0, UINT16_MAX);
      children[i]->coords.w = (uint16_t)clamp_value_signed(computedSizes[i], 0, UINT16_MAX);
      children[i]->coords.h = (uint16_t)clamp_value_signed(secondaryAxisSize, 0, UINT16_MAX);
      break;
    case LayoutType_Vertical:
      children[i]->coords.x = (uint16_t)clamp_value_signed(secondaryAxisPosition, 0, UINT16_MAX);
      children[i]->coords.y = (uint16_t)clamp_value_signed(currentPosition, 0, UINT16_MAX);
      children[i]->coords.w = (uint16_t)clamp_value_signed(secondaryAxisSize, 0, UINT16_MAX);
      children[i]->coords.h = (uint16_t)clamp_value_signed(computedSizes[i], 0, UINT16_MAX);
      break;
    default:
      break;
    }
    children[i]->needsUpdate = false;
    currentPosition += computedSizes[i] + spacing;
  }

  // Set z order
  for (size_t i = 0; i < nChildren; ++i) {
    children[i]->zOrder = nIteration;
  }
}

void ui_traverseAndApplyLayout(ui_ECS_World *world, const ui_ECS_Entity *root)
{
#if defined(UIFW_DEBUG)
  ECS_COMPONENT(world, ui_ECS_BaseComponent);
  ECS_COMPONENT(world, ui_ECS_TransformRelComponent);
  ECS_COMPONENT(world, ui_ECS_LayoutComponent);

  const bool hasBase = ecs_has(world, *root, ui_ECS_BaseComponent);
  const bool hasTransformRel = ecs_has(world, *root, ui_ECS_TransformRelComponent);
  const bool hasLayout = ecs_has(world, *root, ui_ECS_LayoutComponent);

  ui_Assert(hasBase, "ui_traverseAndApplyLayout requires `root` to contain a "
                     "ui_ECS_BaseComponent");

  ui_Assert(hasTransformRel, "ui_traverseAndApplyLayout requires `root` to contain a "
                             "ui_ECS_TransformRelComponent");

  ui_Assert(hasLayout, "ui_traverseAndApplyLayout requires `root` to contain a "
                       "ui_ECS_LayoutComponent");
#endif

  ui_Queue queue;

  ui_queueInit(&queue, sizeof(ui_ECS_Entity));
  ui_queuePush(&queue, root);

  size_t i = 0;

  while (!ui_queueEmpty(&queue)) {
    ui_ECS_Entity currentEntity;
    ui_queuePeek(&queue, &currentEntity);

    layout_children(&currentEntity, world, &queue, i);

    ui_queuePop(&queue, &currentEntity);
    ++i;
  }
}
