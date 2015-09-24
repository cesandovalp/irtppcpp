#include <estimation/estep.h>
#include <type/parameter.h>

namespace irtpp
{

  //void estep(model * m, Matrix<double> * f, Matrix<double> * r, Matrix<double> * weights,
  //           Matrix<double> * probability, dataset * d)
  void estep(e_parameter param)
  {
    double sum;
    int i, k, items, counter_set;
    double * faux;
    Matrix<char>* bitset_list;
    int * counter_temp;
    Matrix<int> * frequency_list;

    faux = new double[param.weights->nR()];
    items = param.d->countItems();
    bitset_list = param.d->getBitsetList();
    frequency_list = param.d->getFrequencyList();
    counter_temp = new int[items];

    param.r->reset();
    param.f->reset();

    for (int pattern = 0; pattern < param.d->size; pattern++)
    {
      sum = 0.0;
      //Calculate g*(k) for all the k's
      //first calculate the P for each k and store it in the array f aux
      for (k = 0; k < param.weights->nR(); k++)
      {
        faux[k] = (*(param.weights))(0, k);
        //Calculate the p (iterate over the items in the productory)
        counter_set = 0;

        for (i = 0; i < items; i++)
        {
          if ((*bitset_list)(pattern,i))
          {
            counter_temp[counter_set++] = i + 1;
            faux[k] *= (*(param.probability))(k,i);
          }
          else
          {
            faux[k] *= 1 - (*(param.probability))(k,i);
          }
        }
        //At this point the productory is calculated and faux[k] is equivalent to p(u_j,theta_k)
        //Now multiply by the weight
        sum += faux[k];
      }

      for (k = 0; k < param.weights->nR(); k++)
      {
        faux[k] *= ((*frequency_list)(pattern,0)) / sum; //This is g*_j_k
        (*(param.f))(0, k) += faux[k];

        for (i = 0; i < counter_set; i++)
          (*(param.r))(k, counter_temp[i] - 1) += faux[k];
      }
    }
  }

}