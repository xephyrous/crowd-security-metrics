import torch
import torchvision

model = torchvision.models.detection.maskrcnn_resnet50_fpn(pretrained=True)
model.eval()

def analyze_frame(frame):
    img = torch.tensor(frame).permute(2, 0, 1).float() / 255.0
    img = img.unsqueeze(0)

    with torch.no_grad():
        outputs = model(img)[0]

    detections = {}
    for i in range(len(outputs["boxes"])):
        label = f"person_{i+1}"
        box = outputs["boxes"][i].tolist()
        mask = (outputs["masks"][i][0].mul(255).byte().cpu().numpy()).tolist()
        score = outputs["scores"][i].item()

        if score > 0.7:
            detections[label] = (box, mask, score)

    return detections
