using UnityEngine;
using System.Collections;
using System;

namespace Obi
{
    

    [Serializable]
    public class ObiPDBendTwistConstraintsData : ObiConstraints<ObiPDBendTwistConstraintsBatch>
    {

        public override ObiPDBendTwistConstraintsBatch CreateBatch(ObiPDBendTwistConstraintsBatch source = null)
        {
            return new ObiPDBendTwistConstraintsBatch();
        }
    }
}