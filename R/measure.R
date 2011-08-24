
# True positives
tp = function(cmat) { cmat[2,2]}
# True negatives
tn = function(cmat) { cmat[1,1]}
# False positives
fp = function(cmat) { cmat[1,2]}
# False negatives
fn = function(cmat) { cmat[2,1]}

# Predicted to be true
positives = function(cmat) { sum(cmat[,2]) }
# Predicted to be false
negatives = function(cmat) { sum(cmat[,1]) }
# Really true
trues = function(cmat) { sum(cmat[2,]) }
# Really false
falses = function(cmat) { sum(cmat[1,]) }

################################################################################
.doc <- "
sn package:StabPerf

Sensitivity measure of a series of predictions

DESCRIPTION
Calculated as TP / (TP + TN)

USAGE
sn(cmat)

EXAMPLES
sprintf(\"%4.2f\", sn(contingency.table(reals_vect, predictions_vect)))

ARGUMENTS
 cmat: A confusion/contingency matrix (reals in rows, predictions in columns)
 
VALUE
 [0..1]

SEE.ALSO
contingency.table, sn, sp, fprate, fnrate
"
sn = function(cmat) { tp(cmat) / trues(cmat) }
attr(sn,'doc') <- .doc

# Specificity
sp = function(cmat) { tn(cmat) / falses(cmat) }
# False positive rate
fprate = function(cmat) { fp(cmat) / falses(cmat) }
# False negative rate
fnrate = function(cmat) { fn(cmat) / trues(cmat) }
# Accuracy

accuracy = function(cmat) { (tp(cmat) + tn(cmat)) / sum(cmat) }
# Positive predictive values
ppv = function(cmat) { tp(cmat) / positives(cmat) }
# "Negative predictive values"
npv = function(cmat) { tn(cmat) / negatives(cmat) }
# Performance coefficient
pc = function(cmat) { tp(cmat) / (tp(cmat) + fp(cmat) + fn(cmat)) }
# Probability excess
pe = function(cmat) { sp(cmat) + sn(cmat) - 1 }
# F-measure, F-score
fmeaure = function(cmat) { 2 * sn(cmat) * ppv(cmat) / (sn(cmat) + ppv(cmat)) }
# Matthew's correlation coefficient
mcc = function(cmat) {
  (tp(cmat)*tn(cmat)-fp(cmat)*fn(cmat)) /
    sqrt(positives(cmat)*trues(cmat)*negative(cmat)*falses(cmat))
}
