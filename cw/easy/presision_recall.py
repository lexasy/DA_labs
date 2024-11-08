from sklearn.metrics import precision_score, recall_score
import numpy as np

y_true = np.array([0, 1, 0, 1, 0, 1, 0, 1, 0, 1])
y_pred = np.array([0, 1, 0, 1, 0, 0, 1, 1, 0, 1])

precision = precision_score(y_true, y_pred)
recall = recall_score(y_true, y_pred)

print(f'Precision: {precision:.2f}')
print(f'Recall: {recall:.2f}')
