y_pred = [['sports'], ['wellness', 'exercise', 'motivation'], ['performance', 'nutrition']]
y_true = [['sports', 'teamwork'], ['excercise', 'motivation'], ['nutrition', 'performance']]

presisions = []
recalls = []

for i in range(len(y_pred)):
    TP = 0
    FP = 0
    FN = 0
    visited = []
    for tag in y_pred[i]:
        if tag in y_true[i]:
            TP += 1
        else:
            FP += 1
        visited.append(tag)
    for tag in y_true[i]:
        if tag not in y_pred[i] and tag not in visited:
            FN += 1
    presisions.append(TP / (TP + FP))
    recalls.append(TP / (TP + FN))

print(f"AVG PRESISION: {sum(presisions) / len(presisions)}")
print(f"AVG RECALL: {sum(recalls) / len(recalls)}")