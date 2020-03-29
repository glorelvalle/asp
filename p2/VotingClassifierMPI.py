from sklearn.base import BaseEstimator
from sklearn.base import ClassifierMixin
from sklearn.model_selection import cross_val_score
from sklearn.linear_model import LogisticRegression
from sklearn.naive_bayes import GaussianNB
from sklearn import datasets
from mpi4py import MPI
import numpy as np
import pandas as pd
import operator
np.random.seed(100)

comm = MPI.COMM_WORLD
rank = comm.Get_rank()
p = comm.Get_size()
# Definicion de dataset
dataset = datasets.load_iris()
X, y = dataset.data[:, 1:3], dataset.target
# Clasificadores que vamos a comparar
c1 = LogisticRegression()
c2 = GaussianNB()

class VotingClassifierMPI(BaseEstimator, ClassifierMixin):

    def __init__(self, classifiers, weights=None):
        self.classifiers = classifiers
        self.weights = weights

    def predict_proba(self, X):
        self.probas_ = [classifiers.predict_proba(X) for classifiers in self.classifiers]
        return np.average(self.probas_, axis=0, weights=self.weights)

    def fit(self, X, y):
        # intento de morir
        if rank == 0:
            for classifiers in self.classifiers:
                classifiers.fit(X, y)
                comm.send(classifiers, dest=0, tag=11)
        else:
            for proc in range(1, p):
                comm.recv(source=proc, tag=11)

    def predict(self, X):
        # todavia nada
        self.classes_ = np.asarray([classifiers.predict(X) for classifiers in self.classifiers])
        if self.weights:
            average = self.predict_proba(X)
            m_voting = np.apply_along_axis(lambda x: max(enumerate(x), key=operator.itemgetter(1))[0], axis=1, arr=average)
        else:
            m_voting = np.asarray([np.argmax(np.bincount(self.classes[:,i])) for i in range(self.classes_.shape[1])])
        return m_voting

voting_clf_mpi = VotingClassifierMPI(classifiers=[c1, c2], weights=[1, 1])
i = 0
df = pd.DataFrame(columns=('w1', 'w2', 'mean', 'std'))
for w1 in range(1, 4):
    for w2 in range(1, 4):
        if (len(set((w1, w2))) == 1):
            continue
        #t_start = MPI.Wtime()
        scores = cross_val_score(estimator=voting_clf_mpi, X=X, y=y, cv=5, scoring='accuracy', n_jobs=1)
        #t_diff = MPI.Wtime()-t_start
        df.loc[i] = [w1, w2, scores.mean(), scores.std()]
        i += 1
df.sort_values(by=['mean', 'std'], ascending=False)
#print(t_diff)
